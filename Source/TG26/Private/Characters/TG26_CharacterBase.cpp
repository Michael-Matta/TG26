// Copyright © 2026 Teka Games. All Rights Reserved.


#include "TG26/Public/Characters/TG26_CharacterBase.h"

#include "AbilitySystem/TG26_AbilitySystemComponent.h"
#include "Characters/Data/TG26_CharacterStartupData.h"
#include "Components/TG26_ItemAbilityManagerComp.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayTags/TG26_GameplayTagsAbility.h"


ATG26_CharacterBase::ATG26_CharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	GetMesh()->bReceivesDecals = false;
	
	TG26_AbilitySystemComponent = CreateDefaultSubobject<UTG26_AbilitySystemComponent>("TG26_AbilitySystemComponent");
	TG26_ItemAbilityManagerComponent = CreateDefaultSubobject<UTG26_ItemAbilityManagerComp>("TG26_ItemAbilityManagerComponent");
}

void ATG26_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
	TG26_AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
	// Load Starting Ability Data
	GiveStartingAbilities();
}


UAbilitySystemComponent* ATG26_CharacterBase::GetAbilitySystemComponent() const
{
	return TG26_AbilitySystemComponent.Get();
}


void ATG26_CharacterBase::GiveStartingAbilities() const
{
	// Because StartupData is a soft-reference we can't check IsValid()
	if (StartupData.IsNull())
		return;
	
	if (UTG26_CharacterStartupData* LoadedStartupData = StartupData.LoadSynchronous())
	{
		LoadedStartupData->GiveAbilityToComponent(TG26_AbilitySystemComponent);
		LoadedStartupData->GiveStartingItems(TG26_ItemAbilityManagerComponent);
	}
}

void ATG26_CharacterBase::SetMovementState(const EMovementState InMovementState)
{
	MovementState = InMovementState;
}

void ATG26_CharacterBase::AddGameplayTag(const FGameplayTag InTag)
{
	TG26_AbilitySystemComponent->AddLooseGameplayTag(InTag);
}


void ATG26_CharacterBase::RemoveGameplayTag(const FGameplayTag InTag)
{
	TG26_AbilitySystemComponent->RemoveLooseGameplayTag(InTag);
}

void ATG26_CharacterBase::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	
	if (!IsValid(TG26_AbilitySystemComponent)) return;
	
	if (GetCharacterMovement()->IsFalling())
	{
		TG26_AbilitySystemComponent->AddLooseGameplayTag(TG26_GameplayTags::Ability_Movement_Airborne);
		TG26_AbilitySystemComponent->RemoveLooseGameplayTag(TG26_GameplayTags::Ability_Movement_Grounded);
	}
	else if (GetCharacterMovement()->IsMovingOnGround())
	{
		TG26_AbilitySystemComponent->AddLooseGameplayTag(TG26_GameplayTags::Ability_Movement_Grounded);
		TG26_AbilitySystemComponent->RemoveLooseGameplayTag(TG26_GameplayTags::Ability_Movement_Airborne);
		TG26_AbilitySystemComponent->RemoveLooseGameplayTag(TG26_GameplayTags::Ability_Movement_DoubleJump);
	}
}

void ATG26_CharacterBase::SetLinkedLayerToDefault()
{
	if (IsValid(AnimLayerClass))
	{
		GetMesh()->LinkAnimClassLayers(AnimLayerClass);
	}
}