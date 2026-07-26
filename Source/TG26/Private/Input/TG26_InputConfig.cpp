// Copyright © 2026 Teka Games. All Rights Reserved.


#include "Input/TG26_InputConfig.h"

TObjectPtr<UInputAction> UTG26_InputConfig::GetInputActionByTag(const FGameplayTag& InInputTag) const
{
	for (const FTG26_InputActionConfig& InputConfig : TG26_InputActions )
	{
		if (IsValid(InputConfig.InputAction) && InputConfig.InputTag == InInputTag)
		{
			return InputConfig.InputAction;
		}
	}
	return nullptr;
}
