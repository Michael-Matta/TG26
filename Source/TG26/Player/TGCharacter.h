// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "TGCharacter.generated.h"

class UJetpackComponent;
class UInputAction;
struct FInputActionValue;

UCLASS()
class TG26_API ATGCharacter : public ACharacter
{
	GENERATED_BODY()
	
	/** Jetpack Component **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	UJetpackComponent* JetpackComponent;

public:
	// Sets default values for this character's properties
	ATGCharacter();

protected:
	
	
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
