// Copyright © 2026 Teka Games. All Rights Reserved.


#include "TG26_WorldConditionMusicTriggers.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/TextRenderComponent.h"


// Sets default values
ATG26_WorldConditionMusicTriggers::ATG26_WorldConditionMusicTriggers()
{
	PrimaryActorTick.bCanEverTick = false;
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	SetRootComponent(BoxCollision);
	
#if WITH_EDITORONLY_DATA
	EditorLabel = CreateEditorOnlyDefaultSubobject<UTextRenderComponent>(TEXT("EditorLabel"));
	if (EditorLabel) // null outside the editor (e.g. commandlets)
	{
		EditorLabel->SetupAttachment(BoxCollision);
		EditorLabel->SetHorizontalAlignment(EHTA_Center);
		EditorLabel->SetVerticalAlignment(EVRTA_TextTop);
		EditorLabel->SetRelativeLocation(FVector(0.f, 0.f, 150.f));
		EditorLabel->SetTextRenderColor(FColor::Purple);
		EditorLabel->SetWorldSize(32.f);
		EditorLabel->SetHiddenInGame(true);
	}
#endif
	
}

// Called when the game starts or when spawned
void ATG26_WorldConditionMusicTriggers::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATG26_WorldConditionMusicTriggers::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if (IsValid(BoxCollision))
	{
		BoxCollision->SetBoxExtent(BoxExtents);
	}
	
#if WITH_EDITORONLY_DATA
	if (EditorLabel)
	{
		EditorLabel->SetText(FText::FromString(GetActorLabel()));
	}
#endif
	
}


