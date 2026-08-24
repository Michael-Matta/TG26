// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "Utilities/TG26_AnimUtils.h"
#include "TG26_CharacterBase.generated.h"


class UTG26_CharacterStartupData;
class UTG26_AbilitySystemComponent;

UCLASS()
class TG26_API ATG26_CharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATG26_CharacterBase();
	
	//~Begin IAbilitySystemInterface 
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~End IAbilitySystemInterface
	
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(VisibleAnywhere, Category="TG26|Character|AbilitySystem")
	TObjectPtr<UTG26_AbilitySystemComponent> TG26_AbilitySystemComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TG26|Animation", meta=(AllowPrivateAccess=true))
	EMovementState MovementState;
	
	// This may contain assets that we don't want loading immediatly so we make it Soft...
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TG26_StartupData")
	TSoftObjectPtr<UTG26_CharacterStartupData> StartupData;
	
	void GiveStartingAbilities() const;
	
public:
	UFUNCTION(BlueprintCallable, Category="TG26|Animation")
	void SetMovementState(const EMovementState InMovementState);
	
	UFUNCTION(BlueprintCallable, Category="TG26|Character|Abilities|Tags")
	void AddGameplayTag(const FGameplayTag& InTag);
	
	UFUNCTION(BlueprintCallable, Category="TG26|Character|Abilities|Tags")
	void RemoveGameplayTag(const FGameplayTag& InTag);

	UFUNCTION(BlueprintPure, Category="TG26|Animation")
	EMovementState GetMovementState() const {return MovementState;}
};
