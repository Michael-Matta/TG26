// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TG26_ItemSpecBase.h"
#include "TG26_SpawnedItemSpec.generated.h"

class ATG26_ItemBase;
/**
 * 
 */
UCLASS()
class TG26_API UTG26_SpawnedItemSpec : public UTG26_ItemSpecBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Object Class")
	TSoftClassPtr<ATG26_ItemBase> ItemClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Equip")
	FInputAbilitySet EquipAbility;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Equip")
	FName PassiveSocket;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Equip")
	FName ActiveSocket;
	
	virtual UTG26_ItemInstanceBase* CreateItemInstance(UObject* Outer) const override;
	
};
