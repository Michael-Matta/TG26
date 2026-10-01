#include "ActorOptimizer/TGAI_ActorOptimizerSubsystem.h"

#include "ActorOptimizer/TGAI_OptimizationPreset.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SkinnedMeshComponent.h"
#include "Editor.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Engine/CollisionProfile.h"
#include "Engine/ComponentDelegateBinding.h"
#include "FileHelpers.h"
#include "Framework/Notifications/NotificationManager.h"
#include "GameFramework/Actor.h"
#include "GameFramework/MovementComponent.h"
#include "ScopedTransaction.h"
#include "Selection.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "Widgets/Notifications/SNotificationList.h"

DEFINE_LOG_CATEGORY(LogActorOptimizer);

#define LOCTEXT_NAMESPACE "ActorOptimizer"

namespace ActorOptimizer
{
	static const FName NAME_NoOptimize(TEXT("NoOptimize"));

	// Top-level properties that hold the persistent state of each option. Used to check whether the
	// Blueprint construction script writes them (see IsWrittenByConstructionScript).
	static const FName NAME_PrimaryComponentTick(TEXT("PrimaryComponentTick"));
	static const FName NAME_BodyInstance(TEXT("BodyInstance"));
	static const FName NAME_GenerateOverlapEvents(TEXT("bGenerateOverlapEvents"));
	static const FName NAME_CanEverAffectNavigation(TEXT("bCanEverAffectNavigation"));

	static FString BoolToString(bool bValue)
	{
		return bValue ? TEXT("true") : TEXT("false");
	}

	static FString CollisionEnabledToString(ECollisionEnabled::Type Value)
	{
		// StaticEnum<> is not available for this namespaced engine enum.
		switch (Value)
		{
		case ECollisionEnabled::NoCollision:     return TEXT("NoCollision");
		case ECollisionEnabled::QueryOnly:       return TEXT("QueryOnly");
		case ECollisionEnabled::PhysicsOnly:     return TEXT("PhysicsOnly");
		case ECollisionEnabled::QueryAndPhysics: return TEXT("QueryAndPhysics");
		case ECollisionEnabled::ProbeOnly:       return TEXT("ProbeOnly");
		case ECollisionEnabled::QueryAndProbe:   return TEXT("QueryAndProbe");
		default:                                 return FString::Printf(TEXT("Unknown(%d)"), static_cast<int32>(Value));
		}
	}

	static bool FindProtectedTag(const TArray<FName>& Tags, const TArray<FName>& ProtectedTags, FName& OutTag)
	{
		for (const FName& Tag : Tags)
		{
			if (ProtectedTags.Contains(Tag))
			{
				OutTag = Tag;
				return true;
			}
		}
		return false;
	}

	/**
	 * Construction-script-created components (SCS and UCS) are destroyed and re-created every time the
	 * construction script reruns (actor moved, Blueprint recompiled, level reloaded). The engine restores
	 * instance edits from FComponentInstanceDataCache, but deliberately skips any property the UCS itself
	 * wrote (tracked via UCSModifiedProperties). Editing such a property on the instance would silently
	 * revert, so we refuse those edits and report them instead.
	 */
	static bool IsWrittenByConstructionScript(const UActorComponent* Component, FName TopLevelProperty)
	{
		if (!IsValid(Component) || TopLevelProperty.IsNone())
		{
			return false;
		}

		TSet<const FProperty*> UCSModified;
		Component->GetUCSModifiedProperties(UCSModified);
		for (const FProperty* Property : UCSModified)
		{
			if (Property && Property->GetFName() == TopLevelProperty)
			{
				return true;
			}
		}
		return false;
	}

	static FString GetClassDisplayName(const UClass* Class)
	{
		if (!Class)
		{
			return TEXT("None");
		}

		FString Name = Class->GetName();
		if (Cast<UBlueprintGeneratedClass>(Class))
		{
			Name.RemoveFromEnd(TEXT("_C"));
		}
		return Name;
	}

	/** Returns why overlap logic depends on this component, or an empty string if nothing does. */
	static FString FindComponentOverlapDependency(const UPrimitiveComponent* Primitive)
	{
		// Blueprint "On Component Begin Overlap (X)" events. Checked from the class first: the instance delegate is
		// usually bound in the editor too, but only the class binding tells us which event it is.
		const AActor* Owner = Primitive->GetOwner();
		static const FName NAME_OnComponentBeginOverlap = GET_MEMBER_NAME_CHECKED(UPrimitiveComponent, OnComponentBeginOverlap);
		static const FName NAME_OnComponentEndOverlap = GET_MEMBER_NAME_CHECKED(UPrimitiveComponent, OnComponentEndOverlap);
		for (const UClass* Class = Owner ? Owner->GetClass() : nullptr; Class; Class = Class->GetSuperClass())
		{
			const UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(Class);
			if (!BPClass)
			{
				continue;
			}

			for (const UDynamicBlueprintBinding* Binding : BPClass->DynamicBindingObjects)
			{
				const UComponentDelegateBinding* ComponentBinding = Cast<UComponentDelegateBinding>(Binding);
				if (!ComponentBinding)
				{
					continue;
				}

				for (const FBlueprintComponentDelegateBinding& Entry : ComponentBinding->ComponentDelegateBindings)
				{
					if (Entry.DelegatePropertyName != NAME_OnComponentBeginOverlap && Entry.DelegatePropertyName != NAME_OnComponentEndOverlap)
					{
						continue;
					}

					// Match through the component variable so renamed instances still resolve; fall back to the name.
					const FObjectProperty* ComponentProperty = FindFProperty<FObjectProperty>(Owner->GetClass(), Entry.ComponentPropertyName);
					const bool bMatches = ComponentProperty
						? ComponentProperty->GetObjectPropertyValue_InContainer(Owner) == Primitive
						: Entry.ComponentPropertyName == Primitive->GetFName();
					if (bMatches)
					{
						return FString::Printf(TEXT("%s has an 'On Component %s Overlap (%s)' event"), *GetClassDisplayName(BPClass),
							Entry.DelegatePropertyName == NAME_OnComponentBeginOverlap ? TEXT("Begin") : TEXT("End"),
							*Entry.ComponentPropertyName.ToString());
					}
				}
			}
		}

		// Anything else bound to the instance, e.g. AddDynamic in a C++ constructor.
		if (Primitive->OnComponentBeginOverlap.IsBound() || Primitive->OnComponentEndOverlap.IsBound())
		{
			return FString::Printf(TEXT("OnComponentBeginOverlap/EndOverlap is bound on %s"), *Primitive->GetName());
		}
		return FString();
	}

	/**
	 * Returns why the actor uses overlaps, or an empty string if it doesn't. Protection is per actor: overlap logic
	 * often reaches past the component it is bound to (GetOverlappingActors, IsOverlappingActor), so one detected use
	 * protects collision on every component of the actor.
	 */
	static FString FindActorOverlapDependency(const AActor* Actor, TConstArrayView<UActorComponent*> Components)
	{
		static const FName NAME_ReceiveActorBeginOverlap(TEXT("ReceiveActorBeginOverlap"));
		static const FName NAME_ReceiveActorEndOverlap(TEXT("ReceiveActorEndOverlap"));
		const UClass* Class = Actor->GetClass();
		if (Class->IsFunctionImplementedInScript(NAME_ReceiveActorBeginOverlap) || Class->IsFunctionImplementedInScript(NAME_ReceiveActorEndOverlap))
		{
			return FString::Printf(TEXT("%s implements Event ActorBeginOverlap/EndOverlap"), *GetClassDisplayName(Class));
		}

		if (Actor->OnActorBeginOverlap.IsBound() || Actor->OnActorEndOverlap.IsBound())
		{
			return TEXT("OnActorBeginOverlap/OnActorEndOverlap is bound");
		}

		// Includes tag-protected components: a protected trigger still means the actor relies on overlaps.
		for (const UActorComponent* Component : Components)
		{
			const UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(Component);
			if (IsValid(Primitive))
			{
				FString Reason = FindComponentOverlapDependency(Primitive);
				if (!Reason.IsEmpty())
				{
					return Reason;
				}
			}
		}
		return FString();
	}

	/** Runs the enabled options over one selection. Audit and Apply both go through here. */
	class FOptimizerPass
	{
	public:
		FOptimizerPass(const FActorOptimizationSettings& InSettings, bool bInApply, FActorOptimizationReport& InReport)
			: Settings(InSettings)
			, bApply(bInApply)
			, Report(InReport)
		{
			ProtectedTags = Settings.ProtectedTags;
			ProtectedTags.AddUnique(NAME_NoOptimize);
		}

		void ProcessActor(AActor* Actor)
		{
			if (!IsValid(Actor))
			{
				return;
			}

			CurrentActor = Actor;
			bActorModified = false;
			ModifiedComponents.Reset();

			FName Tag;
			if (FindProtectedTag(Actor->Tags, ProtectedTags, Tag))
			{
				AddEntry(EActorOptimizerEntryType::Skipped, nullptr, TEXT("Targeting"), TEXT("Tags"), Tag.ToString(), FString(),
					FString::Printf(TEXT("Actor has protected tag '%s'."), *Tag.ToString()));
				++Report.ActorsSkipped;
				return;
			}

			++Report.ActorsProcessed;

			const bool bActorCollisionProtected = FindProtectedTag(Actor->Tags, Settings.CollisionProtectedTags, Tag);
			const FName ActorCollisionTag = Tag;

			TInlineComponentArray<UActorComponent*> Components;
			Actor->GetComponents(Components);

			OverlapDependency = Settings.bSkipOverlapDependentComponents ? FindActorOverlapDependency(Actor, Components) : FString();

			ProcessActorTick(Actor);

			for (UActorComponent* Component : Components)
			{
				// Transient components are never saved; editor-only components (billboards, arrows) are stripped on cook.
				if (!IsValid(Component) || Component->HasAnyFlags(RF_Transient) || Component->IsEditorOnly())
				{
					continue;
				}

				if (FindProtectedTag(Component->ComponentTags, ProtectedTags, Tag))
				{
					AddEntry(EActorOptimizerEntryType::Skipped, Component, TEXT("Targeting"), TEXT("ComponentTags"), Tag.ToString(), FString(),
						FString::Printf(TEXT("Component has protected tag '%s'."), *Tag.ToString()));
					continue;
				}

				ProcessComponentTick(Component);

				if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(Component))
				{
					FName ComponentCollisionTag;
					if (bActorCollisionProtected || FindProtectedTag(Component->ComponentTags, Settings.CollisionProtectedTags, ComponentCollisionTag))
					{
						if (AnyCollisionOptionEnabled())
						{
							const FName UsedTag = bActorCollisionProtected ? ActorCollisionTag : ComponentCollisionTag;
							AddEntry(EActorOptimizerEntryType::Skipped, Primitive, TEXT("Collision"), TEXT("Tags"), UsedTag.ToString(), FString(),
								FString::Printf(TEXT("Collision-protected tag '%s'; collision, physics and navigation left unchanged."), *UsedTag.ToString()));
						}
					}
					else
					{
						ProcessCollision(Primitive);
					}
				}
			}

			FinishActor();
		}

	private:
		bool AnyCollisionOptionEnabled() const
		{
			return Settings.bSetCollision || Settings.bDisableOverlapEvents || Settings.bDisablePhysicsSimulation || Settings.bDisableAffectNavigation;
		}

		// ---------------- Tick ----------------

		void ProcessActorTick(AActor* Actor)
		{
			FActorTickFunction& Tick = Actor->PrimaryActorTick;

			// bStartWithTickEnabled is the serialized flag. SetActorTickEnabled() alone only changes the runtime
			// tick state, which is not saved, so both are set.
			if (!Tick.bCanEverTick || !Tick.bStartWithTickEnabled)
			{
				return;
			}

			if (Settings.bDisableActorTick)
			{
				if (ProposeChange(Actor, nullptr, NAME_None, TEXT("Tick"), TEXT("PrimaryActorTick.bStartWithTickEnabled"),
					BoolToString(true), BoolToString(false),
					TEXT("Runtime code can still re-enable tick (SetActorTickEnabled in BeginPlay).")))
				{
					Tick.bStartWithTickEnabled = false;
					Actor->SetActorTickEnabled(false);
				}
			}
			else if (Settings.bSetTickInterval && !FMath::IsNearlyEqual(Tick.TickInterval, Settings.TickInterval))
			{
				if (ProposeChange(Actor, nullptr, NAME_None, TEXT("Tick"), TEXT("PrimaryActorTick.TickInterval"),
					FString::SanitizeFloat(Tick.TickInterval), FString::SanitizeFloat(Settings.TickInterval),
					TEXT("Tick logic must use DeltaSeconds rather than assuming one call per frame.")))
				{
					Actor->SetActorTickInterval(Settings.TickInterval);
				}
			}
		}

		void ProcessComponentTick(UActorComponent* Component)
		{
			if (!Settings.bDisableComponentTick && !Settings.bSetTickInterval)
			{
				return;
			}

			FActorComponentTickFunction& Tick = Component->PrimaryComponentTick;
			if (!Tick.bCanEverTick || !Tick.bStartWithTickEnabled)
			{
				return;
			}

			if (Settings.bSkipMovementAndAnimationComponents && (Component->IsA<UMovementComponent>() || Component->IsA<USkinnedMeshComponent>()))
			{
				AddEntry(EActorOptimizerEntryType::Skipped, Component, TEXT("Tick"), TEXT("PrimaryComponentTick"), FString(), FString(),
					TEXT("Movement/animation component; tick left unchanged (bSkipMovementAndAnimationComponents)."));
				return;
			}

			if (Settings.bDisableComponentTick)
			{
				if (ProposeChange(Component, Component, NAME_PrimaryComponentTick, TEXT("Tick"), TEXT("PrimaryComponentTick.bStartWithTickEnabled"),
					BoolToString(true), BoolToString(false),
					TEXT("Runtime code can still re-enable tick (SetComponentTickEnabled / Activate).")))
				{
					Tick.bStartWithTickEnabled = false;
					Component->SetComponentTickEnabled(false);
				}
			}
			else if (!FMath::IsNearlyEqual(Tick.TickInterval, Settings.TickInterval))
			{
				if (ProposeChange(Component, Component, NAME_PrimaryComponentTick, TEXT("Tick"), TEXT("PrimaryComponentTick.TickInterval"),
					FString::SanitizeFloat(Tick.TickInterval), FString::SanitizeFloat(Settings.TickInterval)))
				{
					Component->SetComponentTickInterval(Settings.TickInterval);
				}
			}
		}

		// ---------------- Collision & physics ----------------

		void ProcessCollision(UPrimitiveComponent* Primitive)
		{
			if (Settings.bSetCollision)
			{
				ProcessCollisionMode(Primitive);
			}

			if (Settings.bDisableOverlapEvents && Primitive->GetGenerateOverlapEvents())
			{
				if (ProposeCollisionChange(Primitive, NAME_GenerateOverlapEvents, TEXT("bGenerateOverlapEvents"),
					BoolToString(true), BoolToString(false),
					TEXT("Breaks OnComponentBeginOverlap/EndOverlap and triggers that rely on this component.")))
				{
					Primitive->SetGenerateOverlapEvents(false);
				}
			}

			if (Settings.bDisablePhysicsSimulation && Primitive->BodyInstance.bSimulatePhysics)
			{
				if (ProposeChange(Primitive, Primitive, NAME_BodyInstance, TEXT("Physics"), TEXT("BodyInstance.bSimulatePhysics"),
					BoolToString(true), BoolToString(false),
					TEXT("Object will no longer fall, topple or react to impulses.")))
				{
					Primitive->SetSimulatePhysics(false);
				}
			}

			if (Settings.bDisableAffectNavigation && Primitive->CanEverAffectNavigation())
			{
				if (ProposeChange(Primitive, Primitive, NAME_CanEverAffectNavigation, TEXT("Navigation"), TEXT("bCanEverAffectNavigation"),
					BoolToString(true), BoolToString(false),
					TEXT("Navmesh will ignore this geometry; AI may path through it. Rebuild navigation afterwards.")))
				{
					Primitive->SetCanEverAffectNavigation(false);
				}
			}
		}

		void ProcessCollisionMode(UPrimitiveComponent* Primitive)
		{
			const ECollisionEnabled::Type CurrentEnabled = Primitive->GetCollisionEnabled();
			const FName CurrentProfile = Primitive->GetCollisionProfileName();
			const FString CurrentString = FString::Printf(TEXT("%s (%s)"), *CollisionEnabledToString(CurrentEnabled), *CurrentProfile.ToString());

			bool bResultBlocksPawn = false;
			ECollisionEnabled::Type TargetEnabled = CurrentEnabled;
			FString ProposedString;

			switch (Settings.CollisionMode)
			{
			case EActorOptimizerCollisionMode::NoCollision:
				if (CurrentEnabled == ECollisionEnabled::NoCollision)
				{
					return;
				}
				TargetEnabled = ECollisionEnabled::NoCollision;
				ProposedString = CollisionEnabledToString(TargetEnabled);
				break;

			case EActorOptimizerCollisionMode::QueryOnly:
				// Only ever reduce: QueryOnly removes the physics representation. Never adds queries to NoCollision/ProbeOnly.
				if (!CollisionEnabledHasPhysics(CurrentEnabled))
				{
					return;
				}
				TargetEnabled = ECollisionEnabled::QueryOnly;
				ProposedString = CollisionEnabledToString(TargetEnabled);
				// Character movement uses sweeps (queries), so query-only geometry still blocks pawns.
				bResultBlocksPawn = true;
				break;

			case EActorOptimizerCollisionMode::Profile:
			{
				FCollisionResponseTemplate Template;
				const UCollisionProfile* Profiles = UCollisionProfile::Get();
				if (!IsValid(Profiles) || !Profiles->GetProfileTemplate(Settings.CollisionProfileName, Template))
				{
					if (!bReportedInvalidProfile)
					{
						bReportedInvalidProfile = true;
						AddEntryForActor(nullptr, EActorOptimizerEntryType::Warning, nullptr, TEXT("Collision"), TEXT("CollisionProfileName"),
							Settings.CollisionProfileName.ToString(), FString(), TEXT("Did not change collision on any actor due to unknown collision profile."));
					}
					return;
				}
				if (CurrentProfile == Settings.CollisionProfileName)
				{
					return;
				}
				TargetEnabled = Template.CollisionEnabled;
				ProposedString = FString::Printf(TEXT("%s (%s)"), *CollisionEnabledToString(TargetEnabled), *Settings.CollisionProfileName.ToString());
				bResultBlocksPawn = CollisionEnabledHasQuery(TargetEnabled) && Template.ResponseToChannels.GetResponse(ECC_Pawn) == ECR_Block;
				break;
			}
			}

			// Traversal safety: don't let players fall through floors or walk through walls.
			const bool bCurrentlyBlocksPawn = CollisionEnabledHasQuery(CurrentEnabled) && Primitive->GetCollisionResponseToChannel(ECC_Pawn) == ECR_Block;
			if (Settings.bSkipPawnBlockingComponents && bCurrentlyBlocksPawn && !bResultBlocksPawn)
			{
				AddEntry(EActorOptimizerEntryType::Warning, Primitive, TEXT("Collision"), TEXT("Collision"), CurrentString, ProposedString,
					TEXT("Did not change due to Pawn blocking (bSkipPawnBlockingComponents): pawns would pass through it."));
				return;
			}

			const TCHAR* Note = Settings.CollisionMode == EActorOptimizerCollisionMode::Profile
				? TEXT("Replaces all channel responses with the profile's.")
				: TEXT("Collision profile becomes 'Custom'; channel responses are kept.");

			if (ProposeCollisionChange(Primitive, NAME_BodyInstance, TEXT("Collision"), CurrentString, ProposedString, Note))
			{
				if (Settings.CollisionMode == EActorOptimizerCollisionMode::Profile)
				{
					Primitive->SetCollisionProfileName(Settings.CollisionProfileName);
				}
				else
				{
					Primitive->SetCollisionEnabled(TargetEnabled);
				}
			}
		}

		// ---------------- Bookkeeping ----------------

		/**
		 * Records a proposed change. Returns true only when applying and the change is allowed, in which case
		 * Target has already been Modify()'d and the caller must perform the mutation.
		 */
		bool ProposeChange(UObject* Target, UActorComponent* Component, FName PersistentProperty, const TCHAR* Category, const TCHAR* Property,
			const FString& CurrentValue, const FString& ProposedValue, const FString& Note = FString())
		{
			if (Component && IsWrittenByConstructionScript(Component, PersistentProperty))
			{
				AddEntry(EActorOptimizerEntryType::Warning, Component, Category, Property, CurrentValue, ProposedValue,
					TEXT("Did not change due to Blueprint construction script: it sets this property, so an instance edit would revert on the next rerun. Change it in the Blueprint."));
				return false;
			}

			AddEntry(EActorOptimizerEntryType::Change, Component, Category, Property, CurrentValue, ProposedValue, Note);

			if (!bApply || !IsValid(Target))
			{
				return false;
			}

			if (!bActorModified)
			{
				CurrentActor->Modify();
				bActorModified = true;
			}
			Target->Modify();
			if (Component)
			{
				ModifiedComponents.AddUnique(Component);
			}
			return true;
		}

		/** ProposeChange for collision and overlap-event edits; refuses them on actors that use overlaps. */
		bool ProposeCollisionChange(UPrimitiveComponent* Primitive, FName PersistentProperty, const TCHAR* Property,
			const FString& CurrentValue, const FString& ProposedValue, const FString& Note = FString())
		{
			if (!OverlapDependency.IsEmpty())
			{
				AddEntry(EActorOptimizerEntryType::Warning, Primitive, TEXT("Collision"), Property, CurrentValue, ProposedValue,
					FString::Printf(TEXT("Did not change due to overlap usage on this actor: %s (bSkipOverlapDependentComponents)."), *OverlapDependency));
				return false;
			}
			return ProposeChange(Primitive, Primitive, PersistentProperty, TEXT("Collision"), Property, CurrentValue, ProposedValue, Note);
		}

		void FinishActor()
		{
			if (!bApply || !bActorModified)
			{
				return;
			}

			for (UActorComponent* Component : ModifiedComponents)
			{
				if (!IsValid(Component))
				{
					continue;
				}
				Component->MarkRenderStateDirty();
				// No PreEditChange was issued, so this does not trigger RerunConstructionScripts on the owner
				// (see UActorComponent::ConsolidatedPostEditChange); component pointers stay valid.
				Component->PostEditChange();
			}

			// With World Partition / One File Per Actor this dirties the actor's external package.
			CurrentActor->MarkPackageDirty();
		}

		void AddEntry(EActorOptimizerEntryType Type, const UActorComponent* Component, const TCHAR* Category, const FString& Property,
			const FString& CurrentValue, const FString& ProposedValue, const FString& Note)
		{
			AddEntryForActor(CurrentActor, Type, Component, Category, Property, CurrentValue, ProposedValue, Note);
		}

		/** Pass a null Actor for findings about the settings rather than a specific actor. */
		void AddEntryForActor(AActor* Actor, EActorOptimizerEntryType Type, const UActorComponent* Component, const TCHAR* Category,
			const FString& Property, const FString& CurrentValue, const FString& ProposedValue, const FString& Note)
		{
			FActorOptimizationReportEntry& Entry = Report.Entries.AddDefaulted_GetRef();
			Entry.Type = Type;
			Entry.Category = Category;
			Entry.PropertyName = Property;
			Entry.CurrentValue = CurrentValue;
			Entry.ProposedValue = ProposedValue;
			Entry.Note = Note;
			if (IsValid(Actor))
			{
				Entry.ActorName = Actor->GetActorLabel();
				Entry.Actor = Actor;
			}
			if (IsValid(Component))
			{
				Entry.ComponentName = Component->GetName();
			}

			if (Type == EActorOptimizerEntryType::Change)
			{
				++Report.NumChanges;
			}
			else if (Type == EActorOptimizerEntryType::Warning)
			{
				++Report.NumWarnings;
			}
		}

		const FActorOptimizationSettings& Settings;
		const bool bApply;
		FActorOptimizationReport& Report;

		TArray<FName> ProtectedTags;
		bool bReportedInvalidProfile = false;

		/** Why the current actor uses overlaps (empty if it doesn't); blocks collision changes on all its components. */
		FString OverlapDependency;

		AActor* CurrentActor = nullptr;
		bool bActorModified = false;
		TArray<UActorComponent*, TInlineAllocator<16>> ModifiedComponents;
	};
}

// ---------------------------------------------------------------------------------------------------------------------

void UActorOptimizerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	USelection::SelectionChangedEvent.AddUObject(this, &UActorOptimizerSubsystem::HandleEditorSelectionChanged);
	USelection::SelectNoneEvent.AddUObject(this, &UActorOptimizerSubsystem::HandleEditorSelectNone);
}

void UActorOptimizerSubsystem::Deinitialize()
{
	USelection::SelectionChangedEvent.RemoveAll(this);
	USelection::SelectNoneEvent.RemoveAll(this);

	if (PendingSelectionBroadcastHandle.IsValid())
	{
		FTSTicker::RemoveTicker(PendingSelectionBroadcastHandle);
		PendingSelectionBroadcastHandle.Reset();
	}

	OnSelectionChanged.Clear();
	OnSelectionChangedNative.Clear();

	Super::Deinitialize();
}

TArray<AActor*> UActorOptimizerSubsystem::GetSelectedActors() const
{
	TArray<AActor*> Result;
	if (!GEditor)
	{
		return Result;
	}

	UEditorActorSubsystem* ActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>();
	if (!IsValid(ActorSubsystem))
	{
		return Result;
	}

	Result = ActorSubsystem->GetSelectedLevelActors();
	Result.RemoveAll([](const AActor* Actor) { return !IsValid(Actor); });
	return Result;
}

FActorSelectionSummary UActorOptimizerSubsystem::GetSelectionSummary() const
{
	FActorSelectionSummary Summary;

	// GetSelectedLevelActors() logs an error and returns nothing while PIE is running.
	if (!GEditor || GEditor->PlayWorld)
	{
		Summary.DisplayText = LOCTEXT("SummaryPIE", "Selection unavailable while Play In Editor is running");
		return Summary;
	}

	const TArray<AActor*> Actors = GetSelectedActors();
	Summary.TotalSelected = Actors.Num();

	for (const AActor* Actor : Actors)
	{
		++Summary.CountsByClass.FindOrAdd(ActorOptimizer::GetClassDisplayName(Actor->GetClass()));
	}
	Summary.CountsByClass.ValueSort([](int32 A, int32 B) { return A > B; });

	if (Summary.TotalSelected == 0)
	{
		Summary.DisplayText = LOCTEXT("SummaryNone", "No actors selected");
		return Summary;
	}

	TArray<FString> Parts;
	for (const TPair<FString, int32>& Pair : Summary.CountsByClass)
	{
		Parts.Add(FString::Printf(TEXT("%d %s"), Pair.Value, *Pair.Key));
	}
	Summary.DisplayText = FText::Format(LOCTEXT("SummaryFmt", "{0} {0}|plural(one=actor,other=actors) selected ({1})"),
		Summary.TotalSelected, FText::FromString(FString::Join(Parts, TEXT(", "))));
	return Summary;
}

FActorOptimizationReport UActorOptimizerSubsystem::AuditSelection(const FActorOptimizationSettings& Settings, bool bShowNotification)
{
	return ProcessSelection(Settings, /*bApply*/ false, bShowNotification);
}

FActorOptimizationReport UActorOptimizerSubsystem::ApplyToSelection(const FActorOptimizationSettings& Settings, bool bShowNotification)
{
	return ProcessSelection(Settings, /*bApply*/ true, bShowNotification);
}

FActorOptimizationReport UActorOptimizerSubsystem::ProcessSelection(const FActorOptimizationSettings& Settings, bool bApply, bool bShowNotification)
{
	FActorOptimizationReport Report;
	Report.bApplied = bApply;

	if (!GEditor || GEditor->PlayWorld)
	{
		ShowNotification(LOCTEXT("PIERunning", "Actor Optimizer: stop Play In Editor first."), false);
		return Report;
	}

	const TArray<AActor*> Actors = GetSelectedActors();
	if (Actors.IsEmpty())
	{
		UE_LOG(LogActorOptimizer, Display, TEXT("No actors selected; nothing to do."));
		ShowNotification(LOCTEXT("NothingSelected", "Actor Optimizer: no actors selected."), false);
		return Report;
	}

	{
		// One transaction for the whole apply so a single Ctrl+Z reverts it.
		TOptional<FScopedTransaction> Transaction;
		if (bApply)
		{
			Transaction.Emplace(LOCTEXT("ApplyTransaction", "Actor Optimizer: Apply to Selection"));
		}

		ActorOptimizer::FOptimizerPass Pass(Settings, bApply, Report);
		for (AActor* Actor : Actors)
		{
			Pass.ProcessActor(Actor);
		}

		if (Transaction.IsSet() && Report.NumChanges == 0)
		{
			// Don't leave an empty entry on the undo stack.
			Transaction->Cancel();
		}
	}

	const TCHAR* Verb = bApply ? TEXT("Applied") : TEXT("Audit");
	UE_LOG(LogActorOptimizer, Display, TEXT("%s: %d actors processed, %d skipped, %d %s, %d warnings."),
		Verb, Report.ActorsProcessed, Report.ActorsSkipped, Report.NumChanges, bApply ? TEXT("changes made") : TEXT("changes proposed"), Report.NumWarnings);

	for (const FActorOptimizationReportEntry& Entry : Report.Entries)
	{
		const FString Line = FString::Printf(TEXT("[%s] %s %s%s | %s: %s -> %s %s"),
			*StaticEnum<EActorOptimizerEntryType>()->GetNameStringByValue(static_cast<int64>(Entry.Type)),
			Entry.ActorName.IsEmpty() ? TEXT("(Settings)") : *Entry.ActorName,
			Entry.ComponentName.IsEmpty() ? TEXT("") : TEXT("."), *Entry.ComponentName,
			*Entry.PropertyName, *Entry.CurrentValue, *Entry.ProposedValue, *Entry.Note);

		if (Entry.Type == EActorOptimizerEntryType::Warning)
		{
			UE_LOG(LogActorOptimizer, Warning, TEXT("%s"), *Line);
		}
		else
		{
			UE_LOG(LogActorOptimizer, Log, TEXT("%s"), *Line);
		}
	}

	if (bShowNotification)
	{
		const FText Message = bApply
			? FText::Format(LOCTEXT("ApplyToast", "Actor Optimizer applied {0} changes to {1} actors ({2} skipped, {3} warnings)."),
				Report.NumChanges, Report.ActorsProcessed, Report.ActorsSkipped, Report.NumWarnings)
			: FText::Format(LOCTEXT("AuditToast", "Actor Optimizer audit: {0} proposed changes on {1} actors ({2} skipped, {3} warnings). Nothing modified."),
				Report.NumChanges, Report.ActorsProcessed, Report.ActorsSkipped, Report.NumWarnings);
		ShowNotification(Message, true);
	}

	return Report;
}

bool UActorOptimizerSubsystem::SavePreset(UOptimizationPreset* Preset, const FActorOptimizationSettings& Settings)
{
	if (!IsValid(Preset))
	{
		ShowNotification(LOCTEXT("NoPreset", "Actor Optimizer: no preset asset assigned."), false);
		return false;
	}

	Preset->Modify();
	Preset->Settings = Settings;
	Preset->MarkPackageDirty();

	const bool bSaved = UEditorLoadingAndSavingUtils::SavePackages({ Preset->GetPackage() }, /*bOnlyDirty*/ true);
	ShowNotification(bSaved
		? FText::Format(LOCTEXT("PresetSaved", "Saved preset {0}."), FText::FromString(Preset->GetName()))
		: FText::Format(LOCTEXT("PresetSaveFailed", "Failed to save preset {0}."), FText::FromString(Preset->GetName())), bSaved);
	return bSaved;
}

bool UActorOptimizerSubsystem::LoadPreset(const UOptimizationPreset* Preset, FActorOptimizationSettings& OutSettings) const
{
	if (!IsValid(Preset))
	{
		ShowNotification(LOCTEXT("NoPresetLoad", "Actor Optimizer: no preset asset assigned."), false);
		return false;
	}

	OutSettings = Preset->Settings;
	return true;
}

TArray<FName> UActorOptimizerSubsystem::GetCollisionProfileNames()
{
	TArray<TSharedPtr<FName>> SharedNames;
	UCollisionProfile::GetProfileNames(SharedNames);

	TArray<FName> Names;
	Names.Reserve(SharedNames.Num());
	for (const TSharedPtr<FName>& Name : SharedNames)
	{
		if (Name.IsValid())
		{
			Names.Add(*Name);
		}
	}
	return Names;
}

void UActorOptimizerSubsystem::HandleEditorSelectionChanged(UObject* NewSelection)
{
	// The event fires for every USelection; ignore asset (Content Browser) selection changes.
	if (GEditor && NewSelection && NewSelection == GEditor->GetSelectedObjects())
	{
		return;
	}
	QueueSelectionBroadcast();
}

void UActorOptimizerSubsystem::HandleEditorSelectNone()
{
	QueueSelectionBroadcast();
}

void UActorOptimizerSubsystem::QueueSelectionBroadcast()
{
	// Selection events can fire many times in one frame; broadcast once on the next tick.
	if (!PendingSelectionBroadcastHandle.IsValid())
	{
		PendingSelectionBroadcastHandle = FTSTicker::GetCoreTicker().AddTicker(
			FTickerDelegate::CreateUObject(this, &UActorOptimizerSubsystem::FlushSelectionBroadcast));
	}
}

bool UActorOptimizerSubsystem::FlushSelectionBroadcast(float DeltaTime)
{
	PendingSelectionBroadcastHandle.Reset();

	if (OnSelectionChanged.IsBound() || OnSelectionChangedNative.IsBound())
	{
		const FActorSelectionSummary Summary = GetSelectionSummary();
		OnSelectionChangedNative.Broadcast(Summary);
		OnSelectionChanged.Broadcast(Summary);
	}

	return false; // one-shot
}

void UActorOptimizerSubsystem::ShowNotification(const FText& Message, bool bSuccess)
{
	FNotificationInfo Info(Message);
	Info.ExpireDuration = 5.0f;
	Info.bFireAndForget = true;
	Info.bUseSuccessFailIcons = true;

	TSharedPtr<SNotificationItem> Item = FSlateNotificationManager::Get().AddNotification(Info);
	if (Item.IsValid())
	{
		Item->SetCompletionState(bSuccess ? SNotificationItem::CS_Success : SNotificationItem::CS_Fail);
	}
}

#undef LOCTEXT_NAMESPACE
