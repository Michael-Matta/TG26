// Copyright © 2026 Teka Games. All Rights Reserved.


#include "Items/Specs/TG26_PlayerWeaponSpec.h"

#include "Items/InstanceObjects/TG26_PlayerWeaponInstance.h"

UTG26_ItemInstanceBase* UTG26_PlayerWeaponSpec::CreateItemInstance(UObject* Outer) const
{
	return NewObject<UTG26_PlayerWeaponInstance>(Outer);
}
