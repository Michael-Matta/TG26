// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TG26_EditorLibrary.generated.h"

/**
 * 
 */
UCLASS()
class TG26_EDITOR_API UTG26_EditorLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable)
	static void PrintToScreen();
	
};
