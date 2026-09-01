// Copyright © 2026 Teka Games. All Rights Reserved.


#include "StoryTextSystem/TG26_TextEntryWidget.h"
#include "StoryTextSystem/TG26_PlayerStoryTextSubsystem.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"

void UTG26_TextEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SubmitButton)
	{
		SubmitButton->OnClicked.AddDynamic(this, &UTG26_TextEntryWidget::HandleSubmit);
	}
	
	if (BodyBox)
	{
		if (APlayerController* PC = GetOwningPlayer())
		{
			BodyBox->SetUserFocus(PC);
		}
	}
}

void UTG26_TextEntryWidget::OpenPrompt(FName InPromptTag, const FText& InPromptLabel)
{
	PromptTag = InPromptTag;

	if (PromptText)
	{
		PromptText->SetText(InPromptLabel);
	}
	if (BodyBox)
	{
		BodyBox->SetText(FText::GetEmpty());
	}

	AddToViewport();

	if (APlayerController* PC = GetOwningPlayer())
	{
		FInputModeUIOnly Mode;
		if (BodyBox)
		{
			// Focus the box directly — avoids the "SetKeyboardFocus does
			// nothing in NativeConstruct" problem entirely.
			Mode.SetWidgetToFocus(BodyBox->TakeWidget());
		}
		Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(Mode);
		PC->SetShowMouseCursor(true);
	}
}

void UTG26_TextEntryWidget::HandleSubmit()
{
	if (BodyBox)
	{
		if (UTG26_PlayerStoryTextSubsystem* Log =
				GetGameInstance()->GetSubsystem<UTG26_PlayerStoryTextSubsystem>())
		{
			Log->AddEntry(BodyBox->GetText().ToString(), PromptTag);
		}
	}
	NotifyBlueprintClosing();
	Close();
}


void UTG26_TextEntryWidget::Close()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);
	}
	RemoveFromParent();
}
