// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TG26_CharacterBase.h"
#include "TG26_PlayerCharacter.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class TG26_API ATG26_PlayerCharacter : public ATG26_CharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATG26_PlayerCharacter();
	
protected:
	// Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TG26|Character", meta=(AllowPrivateAccess="True"))
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TG26|Character", meta=(AllowPrivateAccess="True"))
	TObjectPtr<UCameraComponent> CameraComponent;
	
	// Input
	UPROPERTY(EditDefaultsOnly, Category ="TG26|Character|Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category ="TG26|Character|Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditDefaultsOnly, Category ="TG26|Character|Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditDefaultsOnly, Category ="TG26|Character|Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> JumpAction ;
	
	UPROPERTY(EditDefaultsOnly, Category ="TG26|Character|Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> SprintAction ;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category ="TG26|Character|Input", meta=(AllowPrivateAccess=true))
	float WalkingSpeed = 500.0;
	
	UPROPERTY(EditDefaultsOnly, Category ="TG26|Character|Input", meta=(AllowPrivateAccess=true))
	float SprintingSpeed = 800.0;
	
public:
	//This is what the delegate binds too in the constructor
	UFUNCTION()
	void HandleControllerChanged(APawn* Pawn, AController* OldController, AController* NewController);
	
	//Overriding a function
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Matching functions for the input Actions, Jump is bound directly to Character class functions
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Sprint();
	
};
	