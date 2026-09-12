// Copyright © 2026 Teka Games. All Rights Reserved.


#include "Components/TG26_ItemAbilityManagerComp.h"



UTG26_ItemAbilityManagerComp::UTG26_ItemAbilityManagerComp()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UTG26_ItemAbilityManagerComp::BeginPlay()
{
	Super::BeginPlay();
}


void UTG26_ItemAbilityManagerComp::CreateItemInstance(UTG26_ItemSpecBase* InItemSpec)
{
}

UTG26_ItemInstanceBase* UTG26_ItemAbilityManagerComp::GetItemInstance(const FGameplayTag& InItemTag) const
{
	if ( const TObjectPtr<UTG26_ItemInstanceBase>* FoundInstance = CurrentItemMap.Find(InItemTag))
	{
		return FoundInstance->Get();
	};
	return nullptr;
}


void UTG26_ItemAbilityManagerComp::EquipItem(const FGameplayTag& InItemTag)
{
}


void UTG26_ItemAbilityManagerComp::UnEquipItem()
{
}



