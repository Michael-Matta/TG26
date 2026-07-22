// Copyright © 2026 Teka Games. All Rights Reserved.


#include "Animation/TG26_AnimInstanceShared.h"
#include "Characters/TG26_CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Utilities/TG26_AnimUtils.h"

void UTG26_AnimInstanceShared::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation(); // HE REMOVED THIS
	
	OwningPlayer = Cast<ATG26_CharacterBase>(TryGetPawnOwner());
	if (!IsValid(OwningPlayer)){return;};
	
	MovementComponent= OwningPlayer->GetCharacterMovement();
	
	WorldLocation = OwningPlayer->GetActorLocation();
	CardinalDeadzone = 10.f;
	bIsFirstUpdate = true;
	
}

void UTG26_AnimInstanceShared::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds); // HE REMOVED THIS
	
	if (!IsValid(OwningPlayer)){return;};
	
	WorldRotation = OwningPlayer->GetActorRotation();
	WorldAcceleration2D = MovementComponent->GetCurrentAcceleration() * FVector(1.f, 1.f, 0.f);
	
	// Creating a Local Space vector from World Space
	LocalAcceleration2D = WorldRotation.UnrotateVector(WorldAcceleration2D);
	bHasAcceleration = LocalAcceleration2D.SizeSquared2D()>0.0f;

	// Update Velocity Data
	WorldVelocity = OwningPlayer->GetVelocity();
	WorldVelocity2D = WorldVelocity * FVector(1.f, 1.f, 0.f);
	bWasMovingLastUpdate = !WorldVelocity2D.IsZero();
	VelocityDirectionAngle = UTG26_AnimUtils::CalculateDirection(WorldVelocity2D, WorldRotation);
	
	// Cardinal Direction from Angle
	VelocityDirection = CalculateDirectionFromAngle(CardinalDeadzone, bWasMovingLastUpdate);
	
	// Size2D gets a float magnitude out of the vector displacement
	DisplacementLastUpdate = (OwningPlayer->GetActorLocation() - WorldLocation).Size2D();
	WorldLocation = OwningPlayer->GetActorLocation();
	
	// Useful for Motion and Stride Warping
	DisplacementSpeed = (DisplacementLastUpdate != 0.0f) ? (DisplacementLastUpdate / DeltaSeconds) : 0.0f;
	
	if (bIsFirstUpdate)
	{
		DisplacementLastUpdate = 0.0f;
		DisplacementSpeed = 0.0f;
	}
	
	bIsFirstUpdate = false;
	
}

ECardinalDirections UTG26_AnimInstanceShared::CalculateDirectionFromAngle(const float InDeadZone,
	const bool bUseCurrentDirection) const
{
	const float AbsAngle = FMath::Abs(VelocityDirectionAngle);
	float FwdDeadZone = InDeadZone;
	float BwdDeadZone = InDeadZone;
	
	// Are we still moving in the same direction. This widens the deadzone if we are already moving in that direction
	if (bUseCurrentDirection)
	{
		if (VelocityDirection == ECardinalDirections::Forward){FwdDeadZone *= 1.5f;}
		if (VelocityDirection == ECardinalDirections::Backward){BwdDeadZone *= 1.5f;}
	}
	
	// Right = 0 degrees
	// Forward = 90 degrees
	// Confused about the rest
	if (AbsAngle <= 45.f + FwdDeadZone){return ECardinalDirections::Forward;} // Should be greater than
	if (AbsAngle <= 135.f - BwdDeadZone){return ECardinalDirections::Backward;}
	if (VelocityDirectionAngle > 0.0f){return ECardinalDirections::Right;}
	return ECardinalDirections::Left;
}
