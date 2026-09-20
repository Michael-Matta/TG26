// Copyright © 2026 Teka Games. All Rights Reserved.


#include "TG26_MusicManager.h"

#include "MetasoundOutputSubsystem.h"
#include "Components/AudioComponent.h"
#include "WorldConditionSystem/TG26_WorldConditionSubsystem.h"

DEFINE_LOG_CATEGORY(LogTG26MusicManager)

// Sets default values
ATG26_MusicManager::ATG26_MusicManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	MusicAudioComponent = CreateDefaultSubobject<UAudioComponent>("MusicComponent");
}


// Called when the game starts or when spawned
void ATG26_MusicManager::BeginPlay()
{
	Super::BeginPlay();
	
	if ( UTG26_WorldConditionSubsystem* WorldSubsystem = GetWorld()->GetSubsystem<UTG26_WorldConditionSubsystem>() )
	{
		WorldSubsystem->RegisterMusicManager(this);
	}
	else{ UE_LOG(LogTG26MusicManager, Warning,
			TEXT("Failed to register with World Condition Subsystem."));}
	
	MusicAudioComponent->OnAudioPlayStateChanged.AddDynamic(this, &ATG26_MusicManager::HandlePlayStateChanged);
	
}


void ATG26_MusicManager::HandlePlayStateChanged(EAudioComponentPlayState NewState)
{

	if (NewState == EAudioComponentPlayState::Playing)
	{
		// StartWatching();
	}
}
