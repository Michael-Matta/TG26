// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TG26_CharacterBase.h"
#include "TG26_PlayerCharacter.generated.h"

class UTG26_InputConfig;
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
	
	virtual void BeginPlay() override;
	
protected:
	// Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TG26|Character", meta=(AllowPrivateAccess="True"))
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TG26|Character", meta=(AllowPrivateAccess="True"))
	TObjectPtr<UCameraComponent> CameraComponent;
	
	// Input Config - Mapping Context and Input Actions
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TG26|Character|Input", meta=(AllowPrivateAccess="True"))
	TObjectPtr<UTG26_InputConfig> InputConfig;
	
	// Animation
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TG26|Character|Animation", meta=(AllowPrivateAccess="True"))
	TSubclassOf<UAnimInstance> AnimLayerClass;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category ="TG26|Character|Input", meta=(AllowPrivateAccess=true))
	float WalkingSpeed = 500.0;
	
	UPROPERTY(EditDefaultsOnly, Category ="TG26|Character|Input", meta=(AllowPrivateAccess=true))
	float SprintingSpeed = 800.0;
	
	// Matching functions for the input Actions, Jump is bound directly to Character class functions
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void AbilityInputPressed(const FGameplayTag InputTag);
	void AbilityInputReleased(const FGameplayTag InputTag);
	
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void Landed(const FHitResult& Hit) override;
	
	UFUNCTION(BlueprintCallable, Category="TG26|Character|Abilities|Tags")
	void AddGameplayTag(const FGameplayTag& InTag);
	
	UFUNCTION(BlueprintCallable, Category="TG26|Character|Abilities|Tags")
	void RemoveGameplayTag(const FGameplayTag& InTag);
	
};
	