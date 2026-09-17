// Copyright © 2026 Teka Games. All Rights Reserved.


#include "Components/TG26_ItemAbilityManagerComp.h"
#include "AbilitySystem/TG26_AbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/TG26_CharacterBase.h"
#include "Items/InstanceObjects/TG26_SpawnedItemInstance.h"
#include "Items/Specs/TG26_SpawnedItemSpec.h"


UTG26_ItemAbilityManagerComp::UTG26_ItemAbilityManagerComp()
{
	PrimaryComponentTick.bCanEverTick = false;
	CurrentItemTag = FGameplayTag();
}


void UTG26_ItemAbilityManagerComp::BeginPlay()
{
	Super::BeginPlay();
	
	if (AActor* Owner = GetOwner())
	{
		if (ATG26_CharacterBase* TG26_Character = Cast<ATG26_CharacterBase>(Owner))
		{
			OwnerSkeletalMeshComp = TG26_Character->GetMesh();
			TG26_AbilitySystemComponent = Cast<UTG26_AbilitySystemComponent>(TG26_Character->GetAbilitySystemComponent());
		}
		
		if (APlayerController* PC = Cast<APlayerController>(Owner->GetInstigatorController()))
		{
			if (ULocalPlayer* LP = PC->GetLocalPlayer())
			{
				InputSubsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
			}
		}
	}
}


void UTG26_ItemAbilityManagerComp::CreateItemInstance(UTG26_ItemSpecBase* InItemSpec)
{
	if (!InItemSpec || !TG26_AbilitySystemComponent) return;
	
	UTG26_ItemInstanceBase* Instance = InItemSpec->CreateItemInstance(this);
	Instance->Initialize(InItemSpec, TG26_AbilitySystemComponent);
	
	// Optional if instance is spawnable type
	if (UTG26_SpawnedItemInstance* SpawnedItemInstance = Cast<UTG26_SpawnedItemInstance>(Instance))
	{
		SpawnedItemInstance->SpawnAndAttachItem(GetOwner());
		SpawnedItemInstance->GrantEquipAbility();
	}
	else
	{
		Instance->GrantAbilities();	
	}
	CurrentItemMap.Add(InItemSpec->ItemTag, Instance);
}


UTG26_ItemInstanceBase* UTG26_ItemAbilityManagerComp::GetItemInstance(const FGameplayTag InItemTag) const
{
	if (UTG26_ItemInstanceBase* FoundInstance = CurrentItemMap.Find(InItemTag)->Get())
	{
		return FoundInstance;
	};
	return nullptr;
}


void UTG26_ItemAbilityManagerComp::EquipItem(const FGameplayTag InItemTag)
{
	// if (IsValid(CurrentActiveItem) && CurrentActiveItem->SpawnedItemSpec->ItemTag == InItemTag) return;
	
	if (IsValid(CurrentActiveItem))
		UnEquipItem();
	
	if (UTG26_SpawnedItemInstance* ItemToEquip = Cast<UTG26_SpawnedItemInstance>(CurrentItemMap.Find(InItemTag)->Get()))
	{
		ItemToEquip->SpawnedItemActor->AttachToComponent(OwnerSkeletalMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, ItemToEquip->SpawnedItemSpec->ActiveSocket);
		CurrentActiveItem = ItemToEquip;
		CurrentActiveItem->GrantAbilities();
		TG26_AbilitySystemComponent->AddLooseGameplayTag(CurrentActiveItem->SpawnedItemSpec->ItemTag);
		InputSubsystem->AddMappingContext(CurrentActiveItem->SpawnedItemSpec->InputMappingContext, 1);
		CurrentItemTag = InItemTag;
	}
}


void UTG26_ItemAbilityManagerComp::UnEquipItem()
{
	if (!IsValid(CurrentActiveItem)) return;
	
	CurrentActiveItem->SpawnedItemActor->AttachToComponent(OwnerSkeletalMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, CurrentActiveItem->SpawnedItemSpec->PassiveSocket);
	CurrentActiveItem->RemoveAbilities();
	TG26_AbilitySystemComponent->RemoveLooseGameplayTag(CurrentActiveItem->SpawnedItemSpec->ItemTag);
	InputSubsystem->RemoveMappingContext(CurrentActiveItem->SpawnedItemSpec->InputMappingContext);
	CurrentActiveItem=nullptr;
	CurrentItemTag = FGameplayTag();
}



