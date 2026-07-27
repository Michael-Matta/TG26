// Copyright © 2026 Teka Games. All Rights Reserved.


#include "TG26/Public/Characters/TG26_CharacterBase.h"

#include "AbilitySystem/TG26_AbilitySystemComponent.h"


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

void ATG26_CharacterBase::GiveStartingAbilities() const
{
	check(TG26_AbilitySystemComponent);
	for (TSubclassOf<UGameplayAbility> Ability : DefaultAbilities)
	{
		// This is a struct that is being created from scratch
		const FGameplayAbilitySpec AbilitySpec(Ability, 1);
		TG26_AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}

UAbilitySystemComponent* ATG26_CharacterBase::GetAbilitySystemComponent() const
{
	return TG26_AbilitySystemComponent.Get();
}
