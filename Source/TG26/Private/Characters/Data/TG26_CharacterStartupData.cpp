// Copyright © 2026 Teka Games. All Rights Reserved.


#include "Characters/Data/TG26_CharacterStartupData.h"

#include "AbilitySystem/TG26_AbilitySystemComponent.h"

void UTG26_CharacterStartupData::GiveAbilityToComponent(UTG26_AbilitySystemComponent* InTG26_AbilityComponent,
                                                        int32 InAbilityLevel)
{
	check(InTG26_AbilityComponent);
	GiveAbilities(StartupAbilities, InTG26_AbilityComponent, InAbilityLevel);
}

void UTG26_CharacterStartupData::GiveAbilities(TArray<TSubclassOf<UTG26_GameplayAbility>>& AbilitiesToGive,
	UTG26_AbilitySystemComponent* InTG26_AbilityComponent, int32 ApplyLevel)
{
	if (AbilitiesToGive.IsEmpty())
		return;
	
	for (const TSubclassOf<UTG26_GameplayAbility>& Ability : AbilitiesToGive)
	{
		if (!IsValid(Ability))
			return;
		
		// FGameplayAbilitySpec is a struct that is being created from scratch in the argument
		InTG26_AbilityComponent->GiveAbility(FGameplayAbilitySpec (Ability, ApplyLevel, INDEX_NONE, InTG26_AbilityComponent->GetAvatarActor()));
	}
}
