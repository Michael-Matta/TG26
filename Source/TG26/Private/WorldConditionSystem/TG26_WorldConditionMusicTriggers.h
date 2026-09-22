// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TG26_WorldConditionMusicTriggers.generated.h"

class UTG26_WorldConditionSubsystem;
class UTextRenderComponent;
class UBoxComponent;

UCLASS()
class TG26_API ATG26_WorldConditionMusicTriggers : public AActor
{
	GENERATED_BODY()

public:
	ATG26_WorldConditionMusicTriggers();

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category="TG26")
	TObjectPtr<UTG26_WorldConditionSubsystem> WorldConditionSubsystem;
	
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
	
public:
	UPROPERTY(BlueprintReadOnly, Category="TG26|MPC")
	FName Decay = TEXT("Decay");
	
	UPROPERTY(BlueprintReadOnly, Category="TG26|MPC")
	FName Chaos = TEXT("Chaos");
	
	UPROPERTY(BlueprintReadOnly, Category="TG26|MPC")
	FName OpacityDecay = TEXT("OpacityDecay");
	
	UPROPERTY(BlueprintReadOnly, Category="TG26|MPC")
	FName OpacityLife = TEXT("OpacityLife");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TG26|MPC")
	float DecayValue = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TG26|MPC")
	float ChaosValue = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TG26|MPC")
	float OpacityDecayValue = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TG26|MPC")
	float OpacityLifeValue = 0.f;
};
