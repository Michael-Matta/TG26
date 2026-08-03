// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "TG26_InputAbilityData.generated.h"

class UTG26_GameplayAbility;

USTRUCT(BlueprintType)
struct FInputAbilitySet
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ExposeFunctionCategories="InputTag"))
	FGameplayTag InputTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UTG26_GameplayAbility> Ability;
	
	// Because this plain struct is not a UObject there is no validity to it (can't check if it is valid). 
	// So we need to make a function to validate everything before we give it to the ability system component.
	bool IsValid() const;
	
};