// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "TG26_InputConfig.h"
#include "TG26_InputComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TG26_API UTG26_InputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	// Replacing EnhancedInputComponents->BindAction with this
	// Input Action will be mapped to a tag
	
	template<class UserObject, typename FuncType>
	void BindNativeAction(const UTG26_InputConfig* InInputConfig, const FGameplayTag& InInputTag, 
		ETriggerEvent TriggerEvent, UserObject* Object, FuncType Func);
	
	template<class UserObject, typename FuncType>
	void BindAbilities(const UTG26_InputConfig* InInputConfig, UserObject* Object, FuncType PressedFunc, 
		FuncType ReleasedFunc);
};


template <class UserObject, typename FuncType>
void UTG26_InputComponent::BindNativeAction(const UTG26_InputConfig* InInputConfig, const FGameplayTag& InInputTag,
	ETriggerEvent TriggerEvent, UserObject* Object, FuncType Func)
{
	check(InInputConfig); // Crashes if not there
	if (const UInputAction* Action = InInputConfig->GetInputActionByTag(InInputTag))
	{
		// Because we are already inheriting from UEnhancedInputComponent we can Bind Inputs
		BindAction(Action, TriggerEvent, Object, Func);
	}
}

template <class UserObject, typename FuncType>
void UTG26_InputComponent::BindAbilities(const UTG26_InputConfig* InInputConfig, UserObject* Object,
	FuncType PressedFunc, FuncType ReleasedFunc)
{
	check(InInputConfig);
	for (const FTG26_InputActionConfig& AbilityActionConfig : InInputConfig->TG26_AbilityInputActions)
	{
		if (AbilityActionConfig.InputAction && AbilityActionConfig.InputTag.IsValid())
		{
			if (PressedFunc)
				BindAction(AbilityActionConfig.InputAction, AbilityActionConfig.TriggerEvent, Object, PressedFunc);
			if (ReleasedFunc)
				BindAction(AbilityActionConfig.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc);
		}
	}
}
