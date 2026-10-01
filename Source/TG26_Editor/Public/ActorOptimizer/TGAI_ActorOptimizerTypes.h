#pragma once

#include "CoreMinimal.h"
#include "TGAI_ActorOptimizerTypes.generated.h"

TG26_EDITOR_API DECLARE_LOG_CATEGORY_EXTERN(LogActorOptimizer, Log, All);

UENUM(BlueprintType)
enum class EActorOptimizerCollisionMode : uint8
{
	NoCollision,
	QueryOnly,
	/** Apply the profile named in CollisionProfileName. */
	Profile
};

UENUM(BlueprintType)
enum class EActorOptimizerEntryType : uint8
{
	/** A property that will be (audit) or was (apply) changed. */
	Change,
	/** An actor or component excluded by tags or safety rules. */
	Skipped,
	/** A change that was intentionally not made, or a risk the user should review. */
	Warning,
	/** Report-only finding. */
	Info
};

/**
 * Every option the Actor Optimizer can apply. Each option has a bool toggle plus its value fields,
 * so the whole struct can be stored in a UOptimizationPreset.
 */
USTRUCT(BlueprintType)
struct TG26_EDITOR_API FActorOptimizationSettings
{
	GENERATED_BODY()

	// ---------------- Targeting ----------------

	/** Actors (or components, via ComponentTags) with any of these tags are skipped entirely. "NoOptimize" is always protected. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	TArray<FName> ProtectedTags;

	/** Actors/components with any of these tags are skipped for collision, physics and navigation changes only (e.g. traversal geometry). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	TArray<FName> CollisionProtectedTags = { FName(TEXT("Traversal")) };

	/** Don't reduce collision on components that currently block the Pawn channel (floors, walls, ledges). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	bool bSkipPawnBlockingComponents = true;

	/**
	 * PER ACTOR: if an actor uses overlaps anywhere, collision and overlap events are left unchanged on ALL of its
	 * components, not just the one with the overlap event. Tick changes still apply.
	 *
	 * An actor uses overlaps if it has Event ActorBeginOverlap/EndOverlap, or OnComponentBegin/EndOverlap bound on any
	 * of its components (C++ or Blueprint component events). Native C++ overrides (NotifyActorBeginOverlap) and pure
	 * polling (GetOverlappingActors with no bound event) cannot be detected; tag those actors instead.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting", meta = (DisplayName = "Skip Overlap Actors (Whole Actor)"))
	bool bSkipOverlapDependentComponents = true;

	// ---------------- Tick ----------------

	/** Disable the actor's own tick (persists via PrimaryActorTick.bStartWithTickEnabled). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tick")
	bool bDisableActorTick = false;

	/** Disable tick on the actor's components (persists via PrimaryComponentTick.bStartWithTickEnabled). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tick")
	bool bDisableComponentTick = false;

	/** Leave movement components and skinned meshes ticking: disabling them breaks movement and animation. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tick")
	bool bSkipMovementAndAnimationComponents = true;

	/** Throttle tick instead of disabling it. Applied to the actor and to components that are not being disabled. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tick")
	bool bSetTickInterval = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tick", meta = (EditCondition = "bSetTickInterval", ClampMin = "0.0", Units = "s"))
	float TickInterval = 0.2f;

	// ---------------- Collision & physics ----------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	bool bSetCollision = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (EditCondition = "bSetCollision"))
	EActorOptimizerCollisionMode CollisionMode = EActorOptimizerCollisionMode::QueryOnly;

	/** Used when CollisionMode == Profile. See UActorOptimizerSubsystem::GetCollisionProfileNames(). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (EditCondition = "bSetCollision && CollisionMode == EActorOptimizerCollisionMode::Profile"))
	FName CollisionProfileName = FName(TEXT("BlockAll"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	bool bDisableOverlapEvents = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	bool bDisablePhysicsSimulation = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	bool bDisableAffectNavigation = false;
};

/** One row of an audit/apply report. */
USTRUCT(BlueprintType)
struct TG26_EDITOR_API FActorOptimizationReportEntry
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Report")
	EActorOptimizerEntryType Type = EActorOptimizerEntryType::Change;

	/** Outliner label. Empty for findings about the settings themselves (e.g. an unknown collision profile). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Report")
	FString ActorName;

	/** Empty for actor-level entries. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Report")
	FString ComponentName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Report")
	FString Category;

	/** Not named "Property": that shadows Python's built-in and is unreachable as entry.property. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Report")
	FString PropertyName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Report")
	FString CurrentValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Report")
	FString ProposedValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Report")
	FString Note;

	/** Soft so a stored report never keeps an actor alive; lets the widget select/focus the actor. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Report")
	TSoftObjectPtr<AActor> Actor;
};

USTRUCT(BlueprintType)
struct TG26_EDITOR_API FActorOptimizationReport
{
	GENERATED_BODY()

	/** False for audits (dry run), true when changes were written. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Report")
	bool bApplied = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Report")
	int32 ActorsProcessed = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Report")
	int32 ActorsSkipped = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Report")
	int32 NumChanges = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Report")
	int32 NumWarnings = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Report")
	TArray<FActorOptimizationReportEntry> Entries;
};

USTRUCT(BlueprintType)
struct TG26_EDITOR_API FActorSelectionSummary
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Selection")
	int32 TotalSelected = 0;

	/** Class display name (Blueprint classes without the _C suffix) -> count. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Selection")
	TMap<FString, int32> CountsByClass;

	/** e.g. "12 actors selected (8 StaticMeshActor, 3 PointLight, 1 BP_Door)". */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Selection")
	FText DisplayText;
};
