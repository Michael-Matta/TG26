// Copyright © 2026 Teka Games. All Rights Reserved.


#include "TG26_WorldConditionSubsystem.h"
#include "TG26_WorldConditionDevSettings.h"
#include "Materials/MaterialParameterCollection.h"
#include "Audio/TG26_MusicManager.h"
#include "Kismet/KismetMaterialLibrary.h"

DEFINE_LOG_CATEGORY(LogTG26WorldCondition);

void UTG26_WorldConditionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	UE_LOG(LogTG26WorldCondition, Warning, TEXT("Initialize running."));
	
	const UTG26_WorldConditionDevSettings* WorldConditionDevSettings = GetDefault<UTG26_WorldConditionDevSettings>();
	
	if (!WorldConditionDevSettings)
	{
		UE_LOG(LogTG26WorldCondition, Warning, TEXT("Project Settings > TG26 World Condition - Settings Failed to Initialize in Subsystem."));
		return;
	}
	
	MPCAsset = WorldConditionDevSettings->MaterialCollection.LoadSynchronous();
	
	if (!MPCAsset)
	{
		UE_LOG(LogTG26WorldCondition, Warning,
			TEXT("No Material Parameter Collection assigned or not Loading — check Project Settings > TG26 World Condition."));
		return;
	}
	
	SeedWorldConditionsFromCollection();
}

void UTG26_WorldConditionSubsystem::Deinitialize()
{
	MPCAsset = nullptr;
	
	Super::Deinitialize();
}

bool UTG26_WorldConditionSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	// Only runs in these subsystems
	return WorldType == EWorldType::Game||EWorldType::PIE ;
}

void UTG26_WorldConditionSubsystem::SeedWorldConditionsFromCollection()
{
	WorldConditions.Empty();
	
	for (const FCollectionScalarParameter& Param : MPCAsset->ScalarParameters)
	{
		WorldConditions.Add(Param.ParameterName, Param.DefaultValue);
	}
	
	if (MPCAsset->VectorParameters.Num() > 0)
	{
		UE_LOG(LogTG26WorldCondition, Warning,
			TEXT("'%s' has %d vector parameter(s), which are not exposed as world conditions."),
			*MPCAsset->GetName(), MPCAsset->VectorParameters.Num());
	}
}

void UTG26_WorldConditionSubsystem::PushWorldCondition(FName WorldCondition, float Value)
{
	if (MPCAsset)
	{
		UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPCAsset, WorldCondition, Value);
	}
	
	OnConditionLevelChanged.Broadcast(WorldCondition, Value);
}

void UTG26_WorldConditionSubsystem::SetConditionLevel(FName WorldCondition, float InValue)
{
	float& CurrentValue = WorldConditions.FindOrAdd(WorldCondition);
	
	// Epsilon Guard
	if (FMath::IsNearlyEqual(CurrentValue, InValue, KINDA_SMALL_NUMBER)) return;
	
	CurrentValue = InValue;
	
	PushWorldCondition(WorldCondition, CurrentValue);
}


float UTG26_WorldConditionSubsystem::GetConditionLevel(FName WorldCondition) const
{
	if (const float* Value = WorldConditions.Find(WorldCondition))
	{
		return *Value;
	}
	
	return 0.f;
}


void UTG26_WorldConditionSubsystem::RegisterMusicManager(ATG26_MusicManager* InMusicManager)
{
	if (!IsValid(InMusicManager))
	{
		UE_LOG(LogTG26WorldCondition, Warning, TEXT("RegisterMusicManager called with an invalid actor — ignoring."));
		return;
	}
	if (MusicManager.IsValid() && MusicManager.Get() != InMusicManager)
	{
		UE_LOG(LogTG26WorldCondition, Warning,
			TEXT("Second Music Manager registration attempt — '%s' is trying to replace '%s'. Only one should be placed per level."),
			*InMusicManager->GetName(), *MusicManager->GetName());
		return;
	}
	
	MusicManager = InMusicManager;
}
