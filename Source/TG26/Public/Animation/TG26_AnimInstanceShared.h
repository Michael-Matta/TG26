// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TG26_AnimInstanceBase.h"
#include "TG26_AnimInstanceShared.generated.h"


enum class ECardinalDirections : uint8;
class ATG26_CharacterBase;
class UCharacterMovementComponent;

UCLASS()
class TG26_API UTG26_AnimInstanceShared : public UTG26_AnimInstanceBase
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	UPROPERTY(Transient)
	TObjectPtr<ATG26_CharacterBase> OwningPlayer;
	
	UPROPERTY(Transient)
	TObjectPtr<UCharacterMovementComponent> MovementComponent;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="TG26|Data|Locomotion")
	FRotator WorldRotation;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="TG26|Data|Locomotion")
	FVector WorldLocation;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="TG26|Data|Locomotion")
	FVector WorldAcceleration2D;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="TG26|Data|Locomotion")
	FVector LocalAcceleration2D;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="TG26|Data|Locomotion")
	bool bHasAcceleration;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="TG26|Data|Locomotion")
	FVector WorldVelocity;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="TG26|Data|Locomotion")
	FVector WorldVelocity2D;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="TG26|Data|Locomotion")
	bool bWasMovingLastUpdate;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="TG26|Data|Locomotion")
	float VelocityDirectionAngle;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="TG26|Data|Locomotion")
	float CardinalDeadzone;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="TG26|Data|Locomotion")
	bool bIsFirstUpdate;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="TG26|Data|Locomotion")
	ECardinalDirections VelocityDirection;
	
	UFUNCTION()
	ECardinalDirections CalculateDirectionFromAngle(const float InDeadZone, const bool bUseCurrentDirection) const;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="TG26|Data|Locomotion")
	float DisplacementLastUpdate;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="TG26|Data|Locomotion")
	float DisplacementSpeed;
	
protected:
	// DELETE if not used: Not for use here this is an example for Layers or classes outside ABP inheritance
	UFUNCTION(BlueprintPure, Category="TG26_Locomotion", meta=(BlueprintThreadSafe)) //He had this as not ThreadSafe, a likely mistake
	UCharacterMovementComponent* GetMovementComponentThreadSafe() const{return MovementComponent;};
	
};
