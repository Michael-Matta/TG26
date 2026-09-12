// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TG26_SpawnedItemInstance.h"
#include "TG26_PlayerWeaponInstance.generated.h"

class UTG26_PlayerWeaponSpec;
class ATG26_PlayerWeapon;
/**
 * 
 */
UCLASS()
class TG26_API UTG26_PlayerWeaponInstance : public UTG26_SpawnedItemInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TObjectPtr<UTG26_PlayerWeaponSpec> PlayerWeaponSpec;
	
	UPROPERTY()
	TObjectPtr<ATG26_PlayerWeapon> SpawnedWeapon;
	
	virtual void Initialize(UTG26_ItemSpecBase* InItemSpec, UTG26_AbilitySystemComponent* TG26_AbilitySystemComponent) override;
	virtual void SpawnAndAttachItem(AActor* InOwner) override;
};
