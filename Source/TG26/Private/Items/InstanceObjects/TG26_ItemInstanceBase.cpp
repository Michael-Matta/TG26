// Copyright © 2026 Teka Games. All Rights Reserved.


#include "Items/InstanceObjects/TG26_ItemInstanceBase.h"

#include "GameplayAbilitySpec.h"
#include "AbilitySystem/TG26_AbilitySystemComponent.h"
#include "AbilitySystem/Data/TG26_InputAbilityData.h"
#include  "AbilitySystem/TG26_GameplayAbility.h" //Being used for AbilitySet
#include "Items/Specs/TG26_ItemSpecBase.h"

void UTG26_ItemInstanceBase::Initialize(UTG26_ItemSpecBase* InItemSpec, UTG26_AbilitySystemComponent* TG26_AbilitySystemComponent)
{
	ItemSpec = InItemSpec;
	OwningASC = TG26_AbilitySystemComponent;
}


void UTG26_ItemInstanceBase::GrantAbilities()
{
	if (!OwningASC) return;
	
	for (const FInputAbilitySet& AbilitySet : ItemSpec->ItemAbilities)
	{
		if (!AbilitySet.IsValid()) continue;
		
		FGameplayAbilitySpec Spec(AbilitySet.Ability, 1, INDEX_NONE, OwningASC->GetAvatarActor());
		Spec.GetDynamicSpecSourceTags().AddTag(AbilitySet.InputTag); //This connects input button presses to this action
		ItemAbilitySpecHandles.Add(OwningASC->GiveAbility(Spec)); // Creates a spec handle
	}
	
}


void UTG26_ItemInstanceBase::RemoveAbilities()
{
	if (!OwningASC) return;
	
	for (FGameplayAbilitySpecHandle& ItemAbilityHandle : ItemAbilitySpecHandles)
	{
		if (ItemAbilityHandle.IsValid())
			OwningASC->ClearAbility(ItemAbilityHandle);
	}
	
	ItemAbilitySpecHandles.Empty();
}
