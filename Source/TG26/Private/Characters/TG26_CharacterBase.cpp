// Copyright © 2026 Teka Games. All Rights Reserved.


#include "TG26/Public/Characters/TG26_CharacterBase.h"

#include "AbilitySystem/TG26_AbilitySystemComponent.h"
#include "Characters/Data/TG26_CharacterStartupData.h"


ATG26_CharacterBase::ATG26_CharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	GetMesh()->bReceivesDecals = false;
	
	TG26_AbilitySystemComponent = CreateDefaultSubobject<UTG26_AbilitySystemComponent>("TG26_AbilitySystemComponent");
	
}

void ATG26_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
	TG26_AbilitySystemComponent->InitAbilityActorInfo(this, this);
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
	}
}

void ATG26_CharacterBase::SetMovementState(const EMovementState InMovementState)
{
	MovementState = InMovementState;
}

