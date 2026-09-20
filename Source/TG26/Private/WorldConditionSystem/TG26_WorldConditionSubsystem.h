// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Tickable.h"
#include "TG26_WorldConditionSubsystem.generated.h"


class ATG26_MusicManager;
class UMaterialParameterCollection;

DECLARE_LOG_CATEGORY_EXTERN(LogTG26WorldCondition, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnConditionLevelChanged, FName, Channel, float, NewValue);



UCLASS()
class TG26_API UTG26_WorldConditionSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	// USubsytem 
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
protected:
	// USubsystem
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;
	
public:
	UFUNCTION(BlueprintCallable, Category = "TG26|WorldCondition")
	void SetConditionLevel(FName WorldCondition, float InValue);
	
	UFUNCTION(BlueprintPure, Category = "TG26|WorldConditon")
	float GetConditionLevel(FName WorldCondition) const;
	
	UPROPERTY()
	TWeakObjectPtr<ATG26_MusicManager> MusicManager;
	
	UFUNCTION()
	void RegisterMusicManager(ATG26_MusicManager* InMusicManager);
	
	// Broadcast on every change 
	UPROPERTY(BlueprintAssignable, Category = "TG26|WorldCondition")
	FOnConditionLevelChanged OnConditionLevelChanged;
	
	UPROPERTY()
	TObjectPtr<UMaterialParameterCollection> MPCAsset;
	
private:
	UPROPERTY()
	TMap<FName, float> WorldConditions;
	

	// Populates WorldConditions from the collection's scalar parameters and their defaults.
	void SeedWorldConditionsFromCollection();
	
	// Writes one condition out to the MPC and listeners using the Delegate
	void PushWorldCondition(FName WorldCondition, float Value);
	
};
