// Copyright © 2026 Teka Games. All Rights Reserved.


#include "Items/InstanceObjects/TG26_PlayerWeaponInstance.h"

#include "Items/Specs/TG26_PlayerWeaponSpec.h"
#include "Items/Weapons/TG26_PlayerWeapon.h"

void UTG26_PlayerWeaponInstance::Initialize(UTG26_ItemSpecBase* InItemSpec, UTG26_AbilitySystemComponent* TG26_AbilitySystemComponent)
{
	Super::Initialize(InItemSpec, TG26_AbilitySystemComponent);
	PlayerWeaponSpec = Cast<UTG26_PlayerWeaponSpec>(InItemSpec);
}

void UTG26_PlayerWeaponInstance::SpawnAndAttachItem(AActor* InOwner)
{
	Super::SpawnAndAttachItem(InOwner);
	SpawnedWeapon = Cast<ATG26_PlayerWeapon>(SpawnedItemActor);
}
