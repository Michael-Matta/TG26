// Copyright © 2026 Teka Games. All Rights Reserved.


#include "AbilitySystem/TG26_AbilitySystemComponent.h"


// Sets default values for this component's properties
UTG26_AbilitySystemComponent::UTG26_AbilitySystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTG26_AbilitySystemComponent::AbilityTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
		return;
	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.Ability && (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)))
		{
			AbilitySpecInputPressed(AbilitySpec);
			TryActivateAbility(AbilitySpec.Handle);
		}
	}
}

void UTG26_AbilitySystemComponent::AbilityTagReleased(const FGameplayTag& InputTag)
{	
	if (!InputTag.IsValid())
		return;
	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.Ability && (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
	
}


