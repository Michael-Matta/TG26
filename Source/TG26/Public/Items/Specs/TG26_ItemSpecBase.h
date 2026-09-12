// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/TG26_InputAbilityData.h"
#include "Engine/DataAsset.h"
#include "TG26_ItemSpecBase.generated.h"

class UTG26_ItemInstanceBase;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class TG26_API UTG26_ItemSpecBase : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Abilities")
	TArray<FInputAbilitySet> ItemAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tag")
	FGameplayTag ItemTag;
	
	// Factory Method
	virtual UTG26_ItemInstanceBase* CreateItemInstance(UObject* Outer) const;

};
