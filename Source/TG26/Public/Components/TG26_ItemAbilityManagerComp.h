// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/Specs/TG26_ItemSpecBase.h"
#include "GameplayTagContainer.h"
#include "TG26_ItemAbilityManagerComp.generated.h"

class ATG26_CharacterBase;
class UTG26_ItemInstanceBase;
class UTG26_SpawnedItemInstance;
class UEnhancedInputLocalPlayerSubsystem;
class UTG26_AbilitySystemComponent;
class UTG26_ItemSpecBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TG26_API UTG26_ItemAbilityManagerComp : public UActorComponent
{
	GENERATED_BODY()

public:
	UTG26_ItemAbilityManagerComp();
	
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void CreateItemInstance(UTG26_ItemSpecBase* InItemSpec);
	
	UFUNCTION(BlueprintCallable)
	void EquipItem(const FGameplayTag InItemTag);
	
	UFUNCTION(BlueprintCallable)
	void UnEquipItem();

	UFUNCTION(BlueprintCallable)
	UTG26_ItemInstanceBase* GetItemInstance(const FGameplayTag InItemTag) const;
	
	UFUNCTION(BlueprintCallable)
	UTG26_SpawnedItemInstance* GetActiveItem() const {return CurrentActiveItem;}
	
	template<typename T>
	T* GetEquippedItemInstance() const
	{
		return Cast<T>(CurrentActiveItem);
	}
	
	template<typename T>
	T* GetTypedItemInstance( const FGameplayTag InItemTag) const
	{
		if (UTG26_ItemInstanceBase* Base = GetItemInstance(InItemTag))
		{
			return Cast<T>(Base);
		}
		return nullptr;
	}
	
	
protected:
	
	UPROPERTY()
	TObjectPtr<UTG26_AbilitySystemComponent> TG26_AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> OwnerSkeletalMeshComp;
	
	UPROPERTY()
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> InputSubsystem;
	
	UPROPERTY()
	TObjectPtr<UTG26_SpawnedItemInstance> CurrentActiveItem;
	
	UPROPERTY()
	FGameplayTag CurrentItemTag;
	
	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<UTG26_ItemInstanceBase>> CurrentItemMap;
	
	UPROPERTY()
	TObjectPtr<ATG26_CharacterBase> TG26_Character;
};
