// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprints/TG26_EditorLibrary.h"

void UTG26_EditorLibrary::PrintToScreen()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Testing Functionality"));
	}
}
