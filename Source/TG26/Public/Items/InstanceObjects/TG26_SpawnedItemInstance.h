// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TG26_ItemInstanceBase.h"
#include "TG26_SpawnedItemInstance.generated.h"

class UTG26_SpawnedItemSpec;
/**
 * 
 */
UCLASS()
class TG26_API UTG26_SpawnedItemInstance : public UTG26_ItemInstanceBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	UTG26_SpawnedItemSpec* SpawnedItemSpec;
	
	UPROPERTY()
	FGameplayAbilitySpecHandle EquipAbilitySpecHandle;
	
	UPROPERTY()
	TObjectPtr<AActor> SpawnedItemActor = nullptr;
	
	
	
	virtual void Initialize(UTG26_ItemSpecBase* InItemSpec, UTG26_AbilitySystemComponent* TG26_AbilitySystemComponent) override;
	virtual void GrantEquipAbility();
	virtual void RemoveEquipAbility();
	
	virtual void SpawnAndAttachItem(AActor* InOwner);
	
};
