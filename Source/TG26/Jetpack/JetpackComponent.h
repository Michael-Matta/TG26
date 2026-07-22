// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JetpackComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJetpackStateChanged, bool, IsActive);

class UCharacterMovementComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TG26_API UJetpackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UJetpackComponent();
	
	UFUNCTION(BlueprintCallable, Category = "JetpackComponent")
	void SetJetpackActive(bool InNewActive);
	
	UFUNCTION(BlueprintPure, Category = "JetpackComponent")
	float GetCurrentFuel() const { return CurrentFuel; }
	
public:
	
	UPROPERTY(BlueprintAssignable, Category = "JetpackComponent")
	FOnJetpackStateChanged OnJetpackActiveChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "JetpackComponent")
	FOnJetpackStateChanged OnJetpackCooldownChangedDelegate;
	
	
protected:
	virtual void BeginPlay() override;

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	
	void ApplyForceToOwner();
	void ConsumeFuel(float InDeltaTime);
	void TryRegenerateFuel(float InDeltaTime);
	
	UCharacterMovementComponent* GetOwnerCharacterMovementComponent() const;
	
	UFUNCTION()
	void ActivateCooldown();
	
	// Function called by the timer
	UFUNCTION()
	void DeactivateCooldown();
	
protected:
	
	/** Force of Jetpack/Jump booster applied to the owner's Movement Component each frame **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JetpackComponent|Physics")
	FVector Force = FVector(0.0f, 0.0f, 1000.0f);	
	
	/** Max Fuel or Stamina **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "JetpackComponent|Stamina")
	float MaxFuel = 100.f;
	
	/** Enter positive values. Amount of Fuel or stamina consumed per second **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JetpackComponent|Stamina", meta = (ClampMin=0.0f))
	float FuelConsumption = 50.f;
	
	/** Amount of Fuel or stamina regenerated per second **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JetpackComponent|Stamina", meta = (ClampMin=0.0f))
	float FuelRegen = 50.f;
	
	/** Duration in seconds in which the Jetpack is in cooldown after fuel is emptied **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "JetpackComponent|Stamina",  meta = (ClampMin=0.0f, Units="s"))
	float CooldownDuration = 1.f;
	
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "JetpackComponent|Stamina")
	float CurrentFuel = 100.f;
	
	UPROPERTY(BlueprintReadOnly, Category = "JetpackComponent")
	bool IsJetpackActive = false;
	
	UPROPERTY(BlueprintReadOnly, Category = "JetpackComponent")
	bool IsCooldownActive = false;
	
	UPROPERTY()
	UCharacterMovementComponent* ownerMovementComponent;
	
private:
	
	FTimerHandle CooldownTimerHandle;
	

	
};
