// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/TG26_PlayerAbility.h"
#include "TG26_SprintAbility.generated.h"

/**
 * 
 */
UCLASS()
class TG26_API UTG26_SprintAbility : public UTG26_PlayerAbility
{
	GENERATED_BODY()
	
	UTG26_SprintAbility();
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
};
