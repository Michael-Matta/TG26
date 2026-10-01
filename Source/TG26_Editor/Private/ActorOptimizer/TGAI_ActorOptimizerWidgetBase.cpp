#include "ActorOptimizer/TGAI_ActorOptimizerWidgetBase.h"

#include "ActorOptimizer/TGAI_ActorOptimizerDetailsCustomization.h"
#include "ActorOptimizer/TGAI_ActorOptimizerSubsystem.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/DetailsView.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Editor.h"
#include "Styling/CoreStyle.h"

#define LOCTEXT_NAMESPACE "ActorOptimizerWidget"

namespace
{
	UActorOptimizerSubsystem* GetOptimizerSubsystem()
	{
		return GEditor ? GEditor->GetEditorSubsystem<UActorOptimizerSubsystem>() : nullptr;
	}

	UButton* MakeButton(UWidgetTree* WidgetTree, FName Name, const FText& Label, const FText& ToolTip)
	{
		UButton* Button = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), Name);
		Button->SetToolTipText(ToolTip);

		UTextBlock* Text = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		Text->SetText(Label);
		// UTextBlock defaults to a 24pt game-UI font; match editor buttons instead.
		Text->SetFont(FCoreStyle::GetDefaultFontStyle("Regular", 10));
		Button->AddChild(Text);
		return Button;
	}
}

TSharedRef<SWidget> UActorOptimizerWidgetBase::RebuildWidget()
{
	// The Blueprint's designer layout wins; only build the default one when it's empty.
	if (WidgetTree && !WidgetTree->RootWidget)
	{
		BuildDefaultLayout();
	}
	return Super::RebuildWidget();
}

void UActorOptimizerWidgetBase::BuildDefaultLayout()
{
	const FMargin RowPadding(4.0f);

	UVerticalBox* Root = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("Root"));
	WidgetTree->RootWidget = Root;

	SelectionText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("SelectionText"));
	SelectionText->SetAutoWrapText(true);
	SelectionText->SetFont(FCoreStyle::GetDefaultFontStyle("Bold", 11));
	Root->AddChildToVerticalBox(SelectionText)->SetPadding(RowPadding);

	UHorizontalBox* Buttons = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("Buttons"));
	Root->AddChildToVerticalBox(Buttons)->SetPadding(RowPadding);

	AuditButton = MakeButton(WidgetTree, TEXT("AuditButton"), LOCTEXT("Audit", "Audit"),
		LOCTEXT("AuditTip", "Dry run: lists every change Apply would make. Modifies nothing."));
	ApplyButton = MakeButton(WidgetTree, TEXT("ApplyButton"), LOCTEXT("Apply", "Apply"),
		LOCTEXT("ApplyTip", "Applies the enabled options to the selected actors. One Ctrl+Z reverts it."));
	LoadPresetButton = MakeButton(WidgetTree, TEXT("LoadPresetButton"), LOCTEXT("LoadPreset", "Load Preset"),
		LOCTEXT("LoadPresetTip", "Copies the Active Preset's settings into the options below."));
	SavePresetButton = MakeButton(WidgetTree, TEXT("SavePresetButton"), LOCTEXT("SavePreset", "Save Preset"),
		LOCTEXT("SavePresetTip", "Writes the options below into the Active Preset and saves it."));

	for (UButton* Button : { AuditButton.Get(), ApplyButton.Get(), LoadPresetButton.Get(), SavePresetButton.Get() })
	{
		Buttons->AddChildToHorizontalBox(Button)->SetPadding(FMargin(0.0f, 0.0f, 4.0f, 0.0f));
	}

	SettingsView = WidgetTree->ConstructWidget<UDetailsView>(UDetailsView::StaticClass(), TEXT("SettingsView"));
	SettingsView->CategoriesToShow = { FName(TEXT("Actor Optimizer")) };
	SettingsView->CategoriesToShow.Append(FActorOptimizerDetailsCustomization::GetSettingsCategories());
	// Settings take only the height they need; the report gets the rest.
	UVerticalBoxSlot* SettingsSlot = Root->AddChildToVerticalBox(SettingsView);
	SettingsSlot->SetPadding(RowPadding);
	SettingsSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));

	ReportText = WidgetTree->ConstructWidget<UMultiLineEditableTextBox>(UMultiLineEditableTextBox::StaticClass(), TEXT("ReportText"));
	ReportText->SetIsReadOnly(true);
	UVerticalBoxSlot* ReportSlot = Root->AddChildToVerticalBox(ReportText);
	ReportSlot->SetPadding(RowPadding);
	ReportSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
}

void UActorOptimizerWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	if (AuditButton)
	{
		AuditButton->OnClicked.AddUniqueDynamic(this, &UActorOptimizerWidgetBase::AuditSelection);
	}
	if (ApplyButton)
	{
		ApplyButton->OnClicked.AddUniqueDynamic(this, &UActorOptimizerWidgetBase::ApplyToSelection);
	}
	if (LoadPresetButton)
	{
		LoadPresetButton->OnClicked.AddUniqueDynamic(this, &UActorOptimizerWidgetBase::HandleLoadPresetClicked);
	}
	if (SavePresetButton)
	{
		SavePresetButton->OnClicked.AddUniqueDynamic(this, &UActorOptimizerWidgetBase::HandleSavePresetClicked);
	}
	if (SettingsView)
	{
		SettingsView->SetObject(this);
	}

	UActorOptimizerSubsystem* Subsystem = GetOptimizerSubsystem();
	if (IsValid(Subsystem) && !SelectionChangedHandle.IsValid())
	{
		SelectionChangedHandle = Subsystem->OnSelectionChangedNative.AddUObject(this, &UActorOptimizerWidgetBase::HandleSelectionChanged);
	}

	RefreshSelectionSummary();
	UpdateReportText();
}

void UActorOptimizerWidgetBase::NativeDestruct()
{
	// The subsystem can already be gone during editor shutdown.
	UActorOptimizerSubsystem* Subsystem = GetOptimizerSubsystem();
	if (IsValid(Subsystem))
	{
		Subsystem->OnSelectionChangedNative.Remove(SelectionChangedHandle);
	}
	SelectionChangedHandle.Reset();

	Super::NativeDestruct();
}

void UActorOptimizerWidgetBase::HandleSelectionChanged(const FActorSelectionSummary& Summary)
{
	if (!bAutoRefreshSelection)
	{
		return;
	}

	SelectionSummary = Summary;
	UpdateSelectionText();
	OnSelectionSummaryUpdated(SelectionSummary);
}

void UActorOptimizerWidgetBase::RefreshSelectionSummary()
{
	UActorOptimizerSubsystem* Subsystem = GetOptimizerSubsystem();
	if (!IsValid(Subsystem))
	{
		return;
	}

	SelectionSummary = Subsystem->GetSelectionSummary();
	UpdateSelectionText();
	OnSelectionSummaryUpdated(SelectionSummary);
}

void UActorOptimizerWidgetBase::AuditSelection()
{
	UActorOptimizerSubsystem* Subsystem = GetOptimizerSubsystem();
	if (!IsValid(Subsystem))
	{
		return;
	}

	LastReport = Subsystem->AuditSelection(Settings);
	UpdateReportText();
	OnReportUpdated(LastReport);
}

void UActorOptimizerWidgetBase::ApplyToSelection()
{
	UActorOptimizerSubsystem* Subsystem = GetOptimizerSubsystem();
	if (!IsValid(Subsystem))
	{
		return;
	}

	LastReport = Subsystem->ApplyToSelection(Settings);
	UpdateReportText();
	OnReportUpdated(LastReport);
}

bool UActorOptimizerWidgetBase::LoadPreset()
{
	UActorOptimizerSubsystem* Subsystem = GetOptimizerSubsystem();
	if (!IsValid(Subsystem) || !Subsystem->LoadPreset(ActivePreset, Settings))
	{
		return false;
	}

	if (SettingsView)
	{
		// The details view doesn't notice values changed from code; rebuild it.
		SettingsView->SetObject(nullptr);
		SettingsView->SetObject(this);
	}
	OnSettingsLoaded();
	return true;
}

bool UActorOptimizerWidgetBase::SavePreset()
{
	UActorOptimizerSubsystem* Subsystem = GetOptimizerSubsystem();
	return IsValid(Subsystem) && Subsystem->SavePreset(ActivePreset, Settings);
}

void UActorOptimizerWidgetBase::HandleLoadPresetClicked()
{
	LoadPreset();
}

void UActorOptimizerWidgetBase::HandleSavePresetClicked()
{
	SavePreset();
}

void UActorOptimizerWidgetBase::UpdateSelectionText()
{
	if (SelectionText)
	{
		SelectionText->SetText(SelectionSummary.DisplayText);
	}
}

void UActorOptimizerWidgetBase::UpdateReportText()
{
	if (ReportText)
	{
		ReportText->SetText(LastReport.Entries.IsEmpty() && LastReport.ActorsProcessed == 0 && LastReport.ActorsSkipped == 0
			? LOCTEXT("NoReport", "Select actors, set the options above, then press Audit to preview the changes.")
			: FormatReport(LastReport));
	}
}

FText UActorOptimizerWidgetBase::FormatReport(const FActorOptimizationReport& Report)
{
	TArray<FString> Lines;
	Lines.Add(FString::Printf(TEXT("%s: %d %s on %d actors (%d skipped), %d warnings."),
		Report.bApplied ? TEXT("APPLIED") : TEXT("AUDIT (nothing modified)"),
		Report.NumChanges, Report.bApplied ? TEXT("changes made") : TEXT("proposed changes"),
		Report.ActorsProcessed, Report.ActorsSkipped, Report.NumWarnings));
	Lines.Add(FString());

	for (const FActorOptimizationReportEntry& Entry : Report.Entries)
	{
		FString Target = Entry.ActorName.IsEmpty() ? TEXT("(Settings)") : Entry.ActorName;
		if (!Entry.ComponentName.IsEmpty())
		{
			Target += TEXT(".") + Entry.ComponentName;
		}

		FString Line = FString::Printf(TEXT("[%s] %s  %s"),
			*StaticEnum<EActorOptimizerEntryType>()->GetDisplayNameTextByValue(static_cast<int64>(Entry.Type)).ToString(),
			*Target, *Entry.PropertyName);
		if (!Entry.CurrentValue.IsEmpty() || !Entry.ProposedValue.IsEmpty())
		{
			Line += FString::Printf(TEXT(": %s -> %s"), *Entry.CurrentValue, *Entry.ProposedValue);
		}
		if (!Entry.Note.IsEmpty())
		{
			Line += TEXT("\n        ") + Entry.Note;
		}
		Lines.Add(MoveTemp(Line));
	}

	return FText::FromString(FString::Join(Lines, TEXT("\n")));
}

#undef LOCTEXT_NAMESPACE
