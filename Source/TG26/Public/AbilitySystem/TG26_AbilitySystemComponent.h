// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TG26_AbilitySystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TG26_API UTG26_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTG26_AbilitySystemComponent();
	
	
	void AbilityTagPressed(const FGameplayTag& InputTag);
	
	void AbilityTagReleased(const FGameplayTag& InputTag);
	
};
