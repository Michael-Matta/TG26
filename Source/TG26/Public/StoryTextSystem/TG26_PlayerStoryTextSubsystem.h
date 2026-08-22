// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TG26_PlayerStoryTextSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FTG26_PlayerTextEntry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, SaveGame)
	FString Body;

	UPROPERTY(BlueprintReadOnly, SaveGame)
	FDateTime EnteredAtUtc = FDateTime::MinValue();

	/** Which prompt/beat produced this, so you can filter on redisplay. */
	UPROPERTY(BlueprintReadOnly, SaveGame)
	FName PromptTag = NAME_None;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTG26_OnTextEntryAdded, int32, Index);

UCLASS()
class TG26_API UTG26_PlayerStoryTextSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/** Returns the index of the new entry, or INDEX_NONE if the body was blank. */
	UFUNCTION(BlueprintCallable, Category = "Story Text")
	int32 AddEntry(const FString& InBody, FName InPromptTag);

	UFUNCTION(BlueprintPure, Category = "Story Text")
	TArray<FTG26_PlayerTextEntry> GetEntriesCopy() const { return Entries; }

	/** Prefer this from C++ — no array copy. */
	const TArray<FTG26_PlayerTextEntry>& GetEntries() const { return Entries; }

	UPROPERTY(BlueprintAssignable, Category = "Story Text")
	FTG26_OnTextEntryAdded OnEntryAdded;

private:
	void FlushToDisk() const;

	UPROPERTY()
	TArray<FTG26_PlayerTextEntry> Entries;
};