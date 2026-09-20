// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TG26_WorldConditionMusicTriggers.generated.h"

UCLASS()
class TG26_API ATG26_WorldConditionMusicTriggers : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATG26_WorldConditionMusicTriggers();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
