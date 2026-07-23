// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Utilities/TG26_AnimUtils.h"
#include "TG26_CharacterBase.generated.h"


UCLASS()
class TG26_API ATG26_CharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATG26_CharacterBase();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TG26|Animation", meta=(AllowPrivateAccess=true))
	EMovementState MovementState;
	
public:
	UFUNCTION(BlueprintPure, Category="Animation")
	EMovementState GetMovementState() const {return MovementState;}

};
