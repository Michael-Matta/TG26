// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/TG26_GameplayAbility.h"
#include "Engine/DataAsset.h"
#include "Items/Specs/TG26_PlayerWeaponSpec.h"
#include "TG26_CharacterStartupData.generated.h"

class UTG26_ItemAbilityManagerComp;
/**
 * 
 */
UCLASS()
class TG26_API UTG26_CharacterStartupData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// For calling in the character
	virtual void GiveAbilityToComponent(UTG26_AbilitySystemComponent* InTG26_AbilityComponent, int32 InAbilityLevel=1);
	virtual void GiveStartingItems(UTG26_ItemAbilityManagerComp* InTG26_AbilityManagerComp);
	
protected:
	// Startup Abilities
	UPROPERTY(EditDefaultsOnly, Category="TG26|StartupData")
	TArray<TSubclassOf<UTG26_GameplayAbility>> StartupAbilities;
	
	UPROPERTY(EditDefaultsOnly, Category="TG26|StartupData|StartingItems")
	TArray<TObjectPtr<UTG26_ItemSpecBase>> StartingItems;
	
	void GiveAbilities(TArray<TSubclassOf<UTG26_GameplayAbility>>& AbilitiesToGive, UTG26_AbilitySystemComponent* InTG26_AbilityComponent, int32 ApplyLevel=1);
	void CreateItemInstances(TArray<TObjectPtr<UTG26_ItemSpecBase>>& InStartingItems, UTG26_ItemAbilityManagerComp* InTG26_ItemAbilityManagerComponent);
};
