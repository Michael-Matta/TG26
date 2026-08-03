// Copyright © 2026 Teka Games. All Rights Reserved.


#include "AbilitySystem/Data/TG26_InputAbilityData.h"
#include "AbilitySystem/TG26_GameplayAbility.h"

bool FInputAbilitySet::IsValid() const
{
	// Because Ability is a TSubclassof it isn't an actual object 
	// so we can't check is valid off of UObject. Just a soft-reference
	// this is the equivalent to Ability!=nullptr 
	return InputTag.IsValid() && Ability;
}
