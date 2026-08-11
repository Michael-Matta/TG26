// Copyright © 2026 Teka Games. All Rights Reserved.


#include "Items/TG26_ItemBase.h"

#include "Components/BoxComponent.h"


// Sets default values
ATG26_ItemBase::ATG26_ItemBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	ItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh"));
	RootComponent = ItemMeshComponent;
	ItemMeshComponent->SetComponentTickEnabled(false);
	
	ItemCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Item Collision"));
	ItemCollision->SetupAttachment(RootComponent);
	ItemCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ItemCollision->SetBoxExtent(FVector(15.0f, 15.0f, 15.0f));
	ItemCollision->SetComponentTickEnabled(false);
	
	
}
