// Copyright © 2026 Teka Games. All Rights Reserved.


#include "Characters/Data/TG26_CharacterStartupData.h"

#include "AbilitySystem/TG26_AbilitySystemComponent.h"
#include "Components/TG26_ItemAbilityManagerComp.h"

void UTG26_CharacterStartupData::GiveAbilityToComponent(UTG26_AbilitySystemComponent* InTG26_AbilityComponent,
                                                        int32 InAbilityLevel)
{
	check(InTG26_AbilityComponent);
	GiveAbilities(StartupAbilities, InTG26_AbilityComponent, InAbilityLevel);
}


void UTG26_CharacterStartupData::GiveStartingItems(UTG26_ItemAbilityManagerComp* InTG26_AbilityManagerComp)
{
	check(InTG26_AbilityManagerComp)
	CreateItemInstances(StartingItems, InTG26_AbilityManagerComp);
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


void UTG26_CharacterStartupData::CreateItemInstances(TArray<TObjectPtr<UTG26_ItemSpecBase>>& InStartingItems, UTG26_ItemAbilityManagerComp* InTG26_ItemAbilityManagerComponent)
{
	if (InStartingItems.IsEmpty()) return;
	
	for (const TObjectPtr<UTG26_ItemSpecBase>& Item : InStartingItems)
	{
		InTG26_ItemAbilityManagerComponent->CreateItemInstance(Item.Get());
	}
	
}
