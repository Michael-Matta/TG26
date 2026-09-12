// Copyright © 2026 Teka Games. All Rights Reserved.


#include "Items/Specs/TG26_SpawnedItemSpec.h"

#include "Items/InstanceObjects/TG26_SpawnedItemInstance.h"

UTG26_ItemInstanceBase* UTG26_SpawnedItemSpec::CreateItemInstance(UObject* Outer) const
{
	return NewObject<UTG26_SpawnedItemInstance>(Outer);
}
