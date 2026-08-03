// Copyright © 2026 Teka Games. All Rights Reserved.


#include "Characters/Data/TG26_PlayerStartupData.h"

#include "AbilitySystem/TG26_AbilitySystemComponent.h"

void UTG26_PlayerStartupData::GiveAbilityToComponent(UTG26_AbilitySystemComponent* InTG26_AbilityComponent,
                                                     int32 InAbilityLevel)
{
	// Keep super to initialize Character parent class abilities
	Super::GiveAbilityToComponent(InTG26_AbilityComponent, InAbilityLevel);
	
	for (const FInputAbilitySet& AbilitySet : PlayerStartupAbilities)
	{
		if (!AbilitySet.IsValid()) continue;
		
		FGameplayAbilitySpec AbilitySpec(AbilitySet.Ability, InAbilityLevel, INDEX_NONE, InTG26_AbilityComponent->GetAvatarActor());
		
		// This will match up the ability with the tag
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilitySet.InputTag);
		
		InTG26_AbilityComponent->GiveAbility(AbilitySpec);
	}
}
