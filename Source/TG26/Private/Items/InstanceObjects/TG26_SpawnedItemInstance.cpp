// Copyright © 2026 Teka Games. All Rights Reserved.


#include "Items/InstanceObjects/TG26_SpawnedItemInstance.h"

#include "GameplayAbilitySpec.h"
#include "AbilitySystem/TG26_AbilitySystemComponent.h"
#include  "AbilitySystem/TG26_GameplayAbility.h" //Being used for AbilitySet
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "GameFramework/Character.h"
#include "Items/TG26_ItemBase.h"
#include "Items/Specs/TG26_SpawnedItemSpec.h"

void UTG26_SpawnedItemInstance::Initialize(UTG26_ItemSpecBase* InItemSpec, UTG26_AbilitySystemComponent* TG26_AbilitySystemComponent)
{
	Super::Initialize(InItemSpec, TG26_AbilitySystemComponent);
	
	SpawnedItemSpec = Cast<UTG26_SpawnedItemSpec>(InItemSpec);
	
}


void UTG26_SpawnedItemInstance::GrantEquipAbility()
{
	if (!SpawnedItemSpec || !OwningASC) return;
	
	if (SpawnedItemSpec->EquipAbility.IsValid())
	{
		FGameplayAbilitySpec Spec(SpawnedItemSpec->EquipAbility.Ability, 1, INDEX_NONE, OwningASC->GetAvatarActor());
		Spec.GetDynamicSpecSourceTags().AddTag(SpawnedItemSpec->EquipAbility.InputTag); //This connects input button presses to this action
		EquipAbilitySpecHandle = OwningASC->GiveAbility(Spec); // Creates a spec handle	
	}
}


void UTG26_SpawnedItemInstance::RemoveEquipAbility()
{
	if (!OwningASC) return;

	if (EquipAbilitySpecHandle.IsValid())
		OwningASC->ClearAbility(EquipAbilitySpecHandle);
	
	EquipAbilitySpecHandle = FGameplayAbilitySpecHandle();
}


void UTG26_SpawnedItemInstance::SpawnAndAttachItem(AActor* InOwner)
{
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	StreamableManager.RequestAsyncLoad(SpawnedItemSpec->ItemClass.ToSoftObjectPath(), 
	FStreamableDelegate::CreateWeakLambda(this,[this, InOwner]()
	{
		// This is where the async loading is occuring
		if (!IsValid(InOwner)) return;
		
		UClass* LoadedItemClass = SpawnedItemSpec->ItemClass.Get();
		if (!LoadedItemClass) return;
		
		// Similar to Blueprints setup for SpawnActor
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = InOwner;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		// This is the node used in BP
		SpawnedItemActor = InOwner->GetWorld()->SpawnActor<ATG26_ItemBase>(LoadedItemClass, SpawnParameters);
	
		if (ACharacter* Character = Cast<ACharacter>(InOwner))
		{
			if (USkeletalMeshComponent* SkelMesh = Character->GetMesh())
			{
				SpawnedItemActor->AttachToComponent(SkelMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SpawnedItemSpec->PassiveSocket); 
			}
		}
	}));
	
	
}
