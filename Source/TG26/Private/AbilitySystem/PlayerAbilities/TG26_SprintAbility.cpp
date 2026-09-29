// Copyright © 2026 Teka Games. All Rights Reserved.


#include "AbilitySystem/PlayerAbilities/TG26_SprintAbility.h"

#include "Characters/TG26_PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayTags/TG26_GameplayTagsAbility.h"

UTG26_SprintAbility::UTG26_SprintAbility()
{
	bActivateAbilityOnGranted = false;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ActivationOwnedTags.AddTag(TG26_GameplayTags::Ability_Movement_Sprinting);
	ActivationBlockedTags.AddTag(TG26_GameplayTags::Ability_Movement_Airborne);
	
}

void UTG26_SprintAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	CommitAbility(Handle, ActorInfo, ActivationInfo);
	
	ATG26_PlayerCharacter* PlayerCharacter= GetPlayerCharacterFromInfo();
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->SetMovementState(EMovementState::Jogging);
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = 850.0f;
	}
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
}
