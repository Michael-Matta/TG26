// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TG26_CharacterStartupData.h"
#include "AbilitySystem/Data/TG26_InputAbilityData.h"
#include "TG26_PlayerStartupData.generated.h"


/**
 * 
 */
UCLASS()
class TG26_API UTG26_PlayerStartupData : public UTG26_CharacterStartupData
{
	GENERATED_BODY()
	
public:
	virtual void GiveAbilityToComponent(UTG26_AbilitySystemComponent* InTG26_AbilityComponent, int32 InAbilityLevel) override;
	
private:

	UPROPERTY(EditDefaultsOnly, Category="TG26|StartupData|AbilitySet")
	TArray<FInputAbilitySet> PlayerStartupAbilities;
};
