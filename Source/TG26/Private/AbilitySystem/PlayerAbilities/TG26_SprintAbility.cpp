// Copyright © 2026 Teka Games. All Rights Reserved.


#include "AbilitySystem/PlayerAbilities/TG26_SprintAbility.h"

#include "Characters/TG26_PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UTG26_SprintAbility::UTG26_SprintAbility()
{
	bActivateAbilityOnGranted = false;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
	
}

void UTG26_SprintAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	//Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	ATG26_PlayerCharacter* PlayerCharacter= GetPlayerCharacterFromInfo();
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->SetMovementState(EMovementState::Jogging);
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = 850.0f;
	}
	
	CommitAbility(Handle, ActorInfo, ActivationInfo);
}


void UTG26_SprintAbility::InputReleased(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	// Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	
	ATG26_PlayerCharacter* PlayerCharacter= GetPlayerCharacterFromInfo();
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->SetMovementState(EMovementState::Walking);
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
	
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}
