// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TG26_WorldConditionMusicTriggers.generated.h"

class UTextRenderComponent;
class UBoxComponent;

UCLASS()
class TG26_API ATG26_WorldConditionMusicTriggers : public AActor
{
	GENERATED_BODY()

public:
	ATG26_WorldConditionMusicTriggers();

	virtual void OnConstruction(const FTransform& Transform) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TG26")
	bool bVisibleInGame = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TG26", meta=(MakeEditWidget = true))
	FVector BoxExtents = FVector(256.f,256.f, 256.f);

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadOnly, Category = "TG26")
	TObjectPtr<UBoxComponent> BoxCollision;

// #if WITH_EDITORONLY_DATA
	UPROPERTY()
	TObjectPtr<class UTextRenderComponent> EditorLabel;
// #endif
	
};
