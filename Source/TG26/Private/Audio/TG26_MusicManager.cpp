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
	
	if (bAutoPlay)MusicAudioComponent->Play();
	
}


void ATG26_MusicManager::HandlePlayStateChanged(EAudioComponentPlayState PlayState)
{

	if (PlayState == EAudioComponentPlayState::Playing)
	{
		if (bShouldWatchEnvelope)
		{
			StartWatching();
		}
	}
	else if (PlayState == EAudioComponentPlayState::Stopped)
	{
		CurrentEnvelope = 0.0f;
	}
}


void ATG26_MusicManager::StartWatching()
{
	if (!IsValid(MusicAudioComponent)) return;
	
	UMetaSoundOutputSubsystem* OutputSubsystem = GetWorld()->GetSubsystem<UMetaSoundOutputSubsystem>();
	if (!OutputSubsystem)
	{
		UE_LOG(LogTG26MusicManager, Warning, TEXT("MetaSoundOutput Subsystem unavailable"));
		return;
	}
	
	bLoggedTypeMismatch = false;
	
	FOnMetasoundOutputValueChanged HandleEnvelopeDelegate;
	HandleEnvelopeDelegate.BindDynamic(this, &ATG26_MusicManager::HandleEnvelopeChanged);
	
	const bool bSuccesfullyWatching = OutputSubsystem->WatchOutput(MusicAudioComponent, EnvelopeOutputName, HandleEnvelopeDelegate);
	
	if (!bSuccesfullyWatching)
	{
		UE_LOG(LogTG26MusicManager, Warning,
			TEXT("WatchOutput failed for '%s' - is the MetaSound playing?"), *EnvelopeOutputName.ToString());
	}
}


void ATG26_MusicManager::HandleEnvelopeChanged(FName OutputName, const FMetaSoundOutput& Output)
{
	float NewValue = 0.f;
	
	if (Output.Get(NewValue))
	{
		if (FMath::IsNearlyEqual(CurrentEnvelope, NewValue, KINDA_SMALL_NUMBER)) return;
		
		CurrentEnvelope = NewValue;
		// Broadcast from a MusicManager BP subscribable delegate
		OnMusicEnvelopeChanged.Broadcast(CurrentEnvelope);
	}
	
	else
	{
		// Output.Get() fails if there is a type mismatch
		if (!bLoggedTypeMismatch)
		{
			// Logs once
			bLoggedTypeMismatch = true;
			UE_LOG(LogTG26MusicManager, Warning,
				TEXT("Output '%s' did not yield a float — check the MetaSound output type."),
				*OutputName.ToString());
		}
	}
}
