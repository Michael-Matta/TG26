#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "Containers/Ticker.h"
#include "ActorOptimizer/TGAI_ActorOptimizerTypes.h"
#include "TGAI_ActorOptimizerSubsystem.generated.h"

class UOptimizationPreset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorOptimizerSelectionChanged, const FActorSelectionSummary&, Summary);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnActorOptimizerSelectionChangedNative, const FActorSelectionSummary&);

/**
 * Editor-only optimizer that operates exclusively on the actors selected in the Level Editor.
 * Audit and Apply share one code path, so an audit shows exactly what Apply would do.
 */
UCLASS()
class TG26_EDITOR_API UActorOptimizerSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/** Selection count and per-class breakdown of the current Level Editor selection. */
	UFUNCTION(BlueprintCallable, Category = "Actor Optimizer")
	FActorSelectionSummary GetSelectionSummary() const;

	/** Dry run: reports every change Apply would make. Modifies nothing. */
	UFUNCTION(BlueprintCallable, Category = "Actor Optimizer")
	FActorOptimizationReport AuditSelection(const FActorOptimizationSettings& Settings, bool bShowNotification = true);

	/** Applies the enabled options to the selected actor instances inside one undoable transaction. */
	UFUNCTION(BlueprintCallable, Category = "Actor Optimizer")
	FActorOptimizationReport ApplyToSelection(const FActorOptimizationSettings& Settings, bool bShowNotification = true);

	/** Copies Settings into Preset and saves the preset package to disk. */
	UFUNCTION(BlueprintCallable, Category = "Actor Optimizer")
	bool SavePreset(UOptimizationPreset* Preset, const FActorOptimizationSettings& Settings);

	/** Returns false (and leaves OutSettings untouched) if Preset is invalid. */
	UFUNCTION(BlueprintCallable, Category = "Actor Optimizer")
	bool LoadPreset(const UOptimizationPreset* Preset, FActorOptimizationSettings& OutSettings) const;

	/** All collision profile names from Project Settings, for a widget combo box. */
	UFUNCTION(BlueprintPure, Category = "Actor Optimizer")
	static TArray<FName> GetCollisionProfileNames();

	/** Fired (coalesced, at most once per frame) when the Level Editor selection changes. */
	UPROPERTY(BlueprintAssignable, Category = "Actor Optimizer")
	FOnActorOptimizerSelectionChanged OnSelectionChanged;

	/** Native counterpart of OnSelectionChanged, used by UActorOptimizerWidgetBase. */
	FOnActorOptimizerSelectionChangedNative OnSelectionChangedNative;

private:
	FActorOptimizationReport ProcessSelection(const FActorOptimizationSettings& Settings, bool bApply, bool bShowNotification);
	TArray<AActor*> GetSelectedActors() const;

	void HandleEditorSelectionChanged(UObject* NewSelection);
	void HandleEditorSelectNone();
	void QueueSelectionBroadcast();
	bool FlushSelectionBroadcast(float DeltaTime);

	static void ShowNotification(const FText& Message, bool bSuccess);

	FTSTicker::FDelegateHandle PendingSelectionBroadcastHandle;
};
