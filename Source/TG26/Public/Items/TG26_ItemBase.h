// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TG26_ItemBase.generated.h"

class UBoxComponent;

UCLASS()
class TG26_API ATG26_ItemBase : public AActor
{
	GENERATED_BODY()

public:
	ATG26_ItemBase();


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TG26|Item")
	TObjectPtr<UStaticMeshComponent> ItemMeshComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetItemCollision, Category="TG26|Item|Collision")
	TObjectPtr<UBoxComponent> ItemCollision;
	
public:
	UFUNCTION(BlueprintGetter)
	UBoxComponent* GetItemCollision() const {return ItemCollision;};

};
