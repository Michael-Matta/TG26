// Fill out your copyright notice in the Description page of Project Settings.


#include "JetpackComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UJetpackComponent::UJetpackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UJetpackComponent::SetJetpackActive(bool InNewActive)
{
	if (IsJetpackActive == InNewActive)
		return;
	
	IsJetpackActive = InNewActive;
	OnJetpackActiveChangedDelegate.Broadcast(InNewActive);
}


void UJetpackComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentFuel = MaxFuel;
	ownerMovementComponent = GetOwnerCharacterMovementComponent();
}


UCharacterMovementComponent* UJetpackComponent::GetOwnerCharacterMovementComponent() const
{
	ACharacter* characterOwner = Cast<ACharacter>(GetOwner());
	return characterOwner ? characterOwner->GetCharacterMovement() : nullptr;
}


void UJetpackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	
	if (IsJetpackActive && CurrentFuel > 0.0f)
	{
		ConsumeFuel(DeltaTime);
		ApplyForceToOwner();		
	}
	
	if (CurrentFuel != MaxFuel)
		TryRegenerateFuel(DeltaTime);
	
}


void UJetpackComponent::ApplyForceToOwner()
{
	if (ownerMovementComponent)
	{
		ownerMovementComponent->AddForce(Force * ownerMovementComponent->Mass);	
	}
	else
	{
		ownerMovementComponent = GetOwnerCharacterMovementComponent();
	}
}


void UJetpackComponent::ConsumeFuel(float InDeltaTime)
{
	const float previousFuel = CurrentFuel;
	CurrentFuel -= FuelConsumption * InDeltaTime;
	
	// Ran out of fuel
	if (CurrentFuel <= 0.0f && previousFuel > 0.0f)
	{
		CurrentFuel = 0.0f;
		ActivateCooldown();
	}
	
}


void UJetpackComponent::TryRegenerateFuel(float InDeltaTime)
{
	if (IsCooldownActive)
		return;
	
	if (!ownerMovementComponent)
	{
		ownerMovementComponent = GetOwnerCharacterMovementComponent();
		return;
	}
	
	if (ownerMovementComponent->IsMovingOnGround() && !IsJetpackActive)
	{
		CurrentFuel += FuelRegen * InDeltaTime;
		// It chooses the CurrentFuel unless it's over MaxFuel then it is MaxFuel
		CurrentFuel = FMath::Min(CurrentFuel, MaxFuel);
	}
	
}


void UJetpackComponent::ActivateCooldown()
{
	IsCooldownActive = true;
	
	FTimerDelegate timerDelegate = FTimerDelegate::CreateUObject(this, &UJetpackComponent::DeactivateCooldown);
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, timerDelegate, CooldownDuration, false);
	
	OnJetpackCooldownChangedDelegate.Broadcast(true);
}


void UJetpackComponent::DeactivateCooldown()
{
	IsCooldownActive = false;
	OnJetpackCooldownChangedDelegate.Broadcast(false);
}

