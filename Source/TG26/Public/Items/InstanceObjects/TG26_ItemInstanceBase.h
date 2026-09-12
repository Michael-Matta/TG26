// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "UObject/Object.h"
#include "TG26_ItemInstanceBase.generated.h"

class UTG26_AbilitySystemComponent;
class UTG26_ItemSpecBase;
/**
 * 
 */
UCLASS()
class TG26_API UTG26_ItemInstanceBase : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TObjectPtr<UTG26_ItemSpecBase> ItemSpec;
	
	UPROPERTY()
	TObjectPtr<UTG26_AbilitySystemComponent> OwningASC;
	
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> ItemAbilitySpecHandles;
	
	virtual void Initialize(UTG26_ItemSpecBase* InItemSpec, UTG26_AbilitySystemComponent* TG26_AbilitySystemComponent);
	virtual void GrantAbilities();
	virtual void RemoveAbilities();
};
