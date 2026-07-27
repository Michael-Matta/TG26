// Copyright © 2026 Teka Games. All Rights Reserved.


#include "AbilitySystem/TG26_PlayerAbility.h"

#include "Characters/TG26_PlayerCharacter.h"
#include "Controllers/TG26_PlayerController.h"

ATG26_PlayerCharacter* UTG26_PlayerAbility::GetPlayerCharacterFromInfo()
{
	if (!PlayerCharacterWeak.IsValid())
	{
		PlayerCharacterWeak = Cast<ATG26_PlayerCharacter>(CurrentActorInfo->AvatarActor);
	}
	
	return PlayerCharacterWeak.IsValid() ? PlayerCharacterWeak.Get(): nullptr;
}

ATG26_PlayerController* UTG26_PlayerAbility::GetPlayerControllerFromInfo()
{
	if (!PlayerControllerWeak.IsValid())
	{
		PlayerControllerWeak = Cast<ATG26_PlayerController>(CurrentActorInfo->AvatarActor);
	}
	
	return PlayerControllerWeak.IsValid() ? PlayerControllerWeak.Get(): nullptr;
}
