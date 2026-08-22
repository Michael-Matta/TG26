// Copyright © 2026 Teka Games. All Rights Reserved.


#include "StoryTextSystem/TG26_TextLogViewWidget.h"
#include "StoryTextSystem/TG26_PlayerStoryTextSubsystem.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

void UTG26_TextLogViewWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Populate();
}

void UTG26_TextLogViewWidget::Populate()
{
	if (!EntryContainer)
	{
		return;
	}
	EntryContainer->ClearChildren();

	const UTG26_PlayerStoryTextSubsystem* Log =
		GetGameInstance()->GetSubsystem<UTG26_PlayerStoryTextSubsystem>();
	if (!Log)
	{
		return;
	}

	for (const FTG26_PlayerTextEntry& Entry : Log->GetEntries())
	{
		UTextBlock* Block = NewObject<UTextBlock>(this);
		Block->SetText(FText::FromString(Entry.Body));
		Block->SetAutoWrapText(true);
		EntryContainer->AddChild(Block);
	}
}