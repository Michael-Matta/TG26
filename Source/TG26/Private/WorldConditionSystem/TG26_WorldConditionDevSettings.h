// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Materials/MaterialParameterCollection.h"
#include "TG26_WorldConditionDevSettings.generated.h"
/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "TG26 World Condition"))
class TG26_API UTG26_WorldConditionDevSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	// UPROPERTY(EditAnywhere, Category = "TG26|WorldCondition")
	// TSoftObjectPtr<UNiagaraParameterCollection> NiagaraCollection;
	
	UPROPERTY(EditAnywhere, Category = "TG26|WorldCondition")
	TSoftObjectPtr<UMaterialParameterCollection> MaterialCollection;
};
