// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TG26_GameplayAbility.generated.h"

class UTG26_AbilitySystemComponent;
/**
 * 
 */
UCLASS()
class TG26_API UTG26_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="TG26|AbilitySystem")
	bool bActivateAbilityOnGranted = false;
	
	UFUNCTION(BlueprintPure, Category="TG26|AbilitySystem")
	UTG26_AbilitySystemComponent* GetOwnerAbilitySystemComponent();

	// This will be used to resolve a bug with Block- block tags it is supposed to be resolved in 5.6
	// virtual bool DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	
};
