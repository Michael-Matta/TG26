// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TG26_GameplayAbility.h"
#include "TG26_PlayerAbility.generated.h"

class ATG26_PlayerController;
class ATG26_PlayerCharacter;
/**
 * 
 */
UCLASS()
class TG26_API UTG26_PlayerAbility : public UTG26_GameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category= "TG26|AbilitySystem")
	ATG26_PlayerCharacter* GetPlayerCharacterFromInfo();
	
	UFUNCTION(BlueprintPure, Category= "TG26|AbilitySystem")
	ATG26_PlayerController* GetPlayerControllerFromInfo();

private:
	TWeakObjectPtr<ATG26_PlayerCharacter> PlayerCharacterWeak;
	TWeakObjectPtr<ATG26_PlayerController> PlayerControllerWeak;
	
};
