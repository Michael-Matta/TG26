// Copyright © 2026 Teka Games. All Rights Reserved.


#include "StoryTextSystem/TG26_PlayerStoryTextSubsystem.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

int32 UTG26_PlayerStoryTextSubsystem::AddEntry(const FString& InBody, FName InPromptTag)
{
	const FString Trimmed = InBody.TrimStartAndEnd();
	if (Trimmed.IsEmpty())
	{
		return INDEX_NONE;
	}

	FTG26_PlayerTextEntry Entry;
	Entry.Body = Trimmed;
	Entry.EnteredAtUtc = FDateTime::UtcNow();
	Entry.PromptTag = InPromptTag;

	const int32 Index = Entries.Add(MoveTemp(Entry));

	FlushToDisk();
	OnEntryAdded.Broadcast(Index);
	return Index;
}

void UTG26_PlayerStoryTextSubsystem::FlushToDisk() const
{
	FString Out;
	for (int32 i = 0; i < Entries.Num(); ++i)
	{
		Out += FString::Printf(TEXT("--- [%d] %s | %s ---\n%s\n\n"),
			i,
			*Entries[i].EnteredAtUtc.ToString(),
			*Entries[i].PromptTag.ToString(),
			*Entries[i].Body);
	}

	// TODO: Save File Location
	const FString Path = FPaths::ProjectSavedDir() / TEXT("TG26_PlaytestTextLog.txt");
	FFileHelper::SaveStringToFile(Out, *Path, FFileHelper::EEncodingOptions::ForceUTF8);
}