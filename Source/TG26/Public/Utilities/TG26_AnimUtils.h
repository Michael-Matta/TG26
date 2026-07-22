// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Uobject/Object.h"
#include "TG26_AnimUtils.generated.h"

UENUM(BlueprintType)
enum class ECardinalDirections:uint8
{
	Forward,
	Backward,
	Right,
	Left
};


UENUM()
enum class EMovementState : uint8
{
	Walking,
	Jogging
};




UCLASS()
class TG26_API UTG26_AnimUtils: public UObject
{
	GENERATED_BODY()
	
public:
	static float CalculateDirection(const FVector& Velocity, const FRotator& BaseRotation);

};
