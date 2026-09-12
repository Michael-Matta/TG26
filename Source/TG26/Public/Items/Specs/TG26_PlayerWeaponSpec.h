// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TG26_SpawnedItemSpec.h"
#include "TG26_PlayerWeaponSpec.generated.h"

class ATG26_ItemBase;
/**
 * 
 */
UCLASS()
class TG26_API UTG26_PlayerWeaponSpec : public UTG26_SpawnedItemSpec
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	TSoftClassPtr<ATG26_ItemBase> Weapon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	FName AttachSocket;
	
	virtual UTG26_ItemInstanceBase* CreateItemInstance(UObject* Outer) const override;
};
