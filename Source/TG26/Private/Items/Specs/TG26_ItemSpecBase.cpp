// Copyright © 2026 Teka Games. All Rights Reserved.


#include "Items/Specs/TG26_ItemSpecBase.h"

#include "Items/InstanceObjects/TG26_ItemInstanceBase.h"

UTG26_ItemInstanceBase* UTG26_ItemSpecBase::CreateItemInstance(UObject* Outer) const
{
	return NewObject<UTG26_ItemInstanceBase>(Outer);
}
