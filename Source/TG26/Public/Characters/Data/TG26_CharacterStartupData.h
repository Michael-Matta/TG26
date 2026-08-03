// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/TG26_GameplayAbility.h"
#include "Engine/DataAsset.h"
#include "TG26_CharacterStartupData.generated.h"

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
	
protected:
	// Startup Abilities
	UPROPERTY(EditDefaultsOnly, Category="TG26|StartupData")
	TArray<TSubclassOf<UTG26_GameplayAbility>> StartupAbilities;
	
	void GiveAbilities(TArray<TSubclassOf<UTG26_GameplayAbility>>& AbilitiesToGive, UTG26_AbilitySystemComponent* InTG26_AbilityComponent, int32 ApplyLevel=1);
};
