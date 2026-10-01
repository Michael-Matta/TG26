#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "ActorOptimizer/TGAI_ActorOptimizerTypes.h"
#include "TGAI_ActorOptimizerWidgetBase.generated.h"

class UButton;
class UDetailsView;
class UMultiLineEditableTextBox;
class UOptimizationPreset;
class UTextBlock;

/**
 * C++ parent for the Actor Optimizer Editor Utility Widget Blueprint.
 *
 * If the Blueprint's designer is empty, a default layout is built in C++ (selection summary, buttons, settings
 * details view, report). To restyle, lay it out in the designer instead, using the BindWidgetOptional names below;
 * any of them that exist are wired up automatically.
 */
UCLASS(Abstract)
class TG26_EDITOR_API UActorOptimizerWidgetBase : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Optimizer", meta = (ShowOnlyInnerProperties))
	FActorOptimizationSettings Settings;

	/** Preset used by LoadPreset / SavePreset. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Optimizer")
	TObjectPtr<UOptimizationPreset> ActivePreset;

	/** Refresh the selection display automatically whenever the Level Editor selection changes. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Optimizer")
	bool bAutoRefreshSelection = true;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Actor Optimizer")
	FActorSelectionSummary SelectionSummary;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Actor Optimizer")
	FActorOptimizationReport LastReport;

	UFUNCTION(BlueprintCallable, Category = "Actor Optimizer")
	void AuditSelection();

	UFUNCTION(BlueprintCallable, Category = "Actor Optimizer")
	void ApplyToSelection();

	/** Copies ActivePreset's settings into Settings. */
	UFUNCTION(BlueprintCallable, Category = "Actor Optimizer")
	bool LoadPreset();

	/** Writes Settings into ActivePreset and saves it. */
	UFUNCTION(BlueprintCallable, Category = "Actor Optimizer")
	bool SavePreset();

	UFUNCTION(BlueprintCallable, Category = "Actor Optimizer")
	void RefreshSelectionSummary();

	/** Multi-line, human-readable version of a report, as shown in ReportText. */
	UFUNCTION(BlueprintPure, Category = "Actor Optimizer")
	static FText FormatReport(const FActorOptimizationReport& Report);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Actor Optimizer")
	void OnSelectionSummaryUpdated(const FActorSelectionSummary& Summary);

	UFUNCTION(BlueprintImplementableEvent, Category = "Actor Optimizer")
	void OnReportUpdated(const FActorOptimizationReport& Report);

	/** Called after LoadPreset so the Blueprint can push the new values into its input widgets. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Actor Optimizer")
	void OnSettingsLoaded();

	UPROPERTY(BlueprintReadOnly, Category = "Actor Optimizer|Widgets", meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> SelectionText;

	UPROPERTY(BlueprintReadOnly, Category = "Actor Optimizer|Widgets", meta = (BindWidgetOptional))
	TObjectPtr<UButton> AuditButton;

	UPROPERTY(BlueprintReadOnly, Category = "Actor Optimizer|Widgets", meta = (BindWidgetOptional))
	TObjectPtr<UButton> ApplyButton;

	UPROPERTY(BlueprintReadOnly, Category = "Actor Optimizer|Widgets", meta = (BindWidgetOptional))
	TObjectPtr<UButton> LoadPresetButton;

	UPROPERTY(BlueprintReadOnly, Category = "Actor Optimizer|Widgets", meta = (BindWidgetOptional))
	TObjectPtr<UButton> SavePresetButton;

	/** Shows this widget's "Actor Optimizer" category plus the settings categories from FActorOptimizerDetailsCustomization. */
	UPROPERTY(BlueprintReadOnly, Category = "Actor Optimizer|Widgets", meta = (BindWidgetOptional))
	TObjectPtr<UDetailsView> SettingsView;

	UPROPERTY(BlueprintReadOnly, Category = "Actor Optimizer|Widgets", meta = (BindWidgetOptional))
	TObjectPtr<UMultiLineEditableTextBox> ReportText;

private:
	void BuildDefaultLayout();
	void UpdateSelectionText();
	void UpdateReportText();
	void HandleSelectionChanged(const FActorSelectionSummary& Summary);

	// Dynamic-delegate targets for the buttons (LoadPreset/SavePreset return bool, which OnClicked can't bind).
	UFUNCTION()
	void HandleLoadPresetClicked();

	UFUNCTION()
	void HandleSavePresetClicked();

	FDelegateHandle SelectionChangedHandle;
};
