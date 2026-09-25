// Copyright © 2026 Teka Games. All Rights Reserved.


#include "TG26/Public/Audio/TG26_MusicManager.h"

#include "MetasoundOutputSubsystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraParameterCollection.h"
#include "Components/AudioComponent.h"
#include "WorldConditionSystem/TG26_WorldConditionSubsystem.h"

DEFINE_LOG_CATEGORY(LogTG26MusicManager)

// Sets default values
ATG26_MusicManager::ATG26_MusicManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));
	
	MusicAudioComponentA = CreateDefaultSubobject<UAudioComponent>("Music Component A for FX");
	MusicAudioComponentA->SetupAttachment(RootComponent);
	
	MusicAudioComponentB = CreateDefaultSubobject<UAudioComponent>("Music Component B");
	MusicAudioComponentB->SetupAttachment(RootComponent);
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
	
	MusicAudioComponentA->OnAudioPlayStateChanged.AddDynamic(this, &ATG26_MusicManager::HandlePlayStateChanged);
	if (bAutoPlay)MusicAudioComponentA->Play();
	
	if (NPCMusicFX)
	{
		NPCMusicFXInstance = UNiagaraFunctionLibrary::GetNiagaraParameterCollection(this, NPCMusicFX);
	}
	
	EnvelopeOutputNameString = EnvelopeOutputName.ToString();
}


UAudioComponent* ATG26_MusicManager::GetMusicAudioComponentA() const
{
	if (!IsValid(MusicAudioComponentA))
	{
		UE_LOG(LogTG26MusicManager, Warning,
			TEXT("Music Audio Component A is not valid"))
		return nullptr;
	}
	
	return MusicAudioComponentA.Get(); 
}


UAudioComponent* ATG26_MusicManager::GetMusicAudioComponentB() const
{
	if (!IsValid(MusicAudioComponentB))
	{
		UE_LOG(LogTG26MusicManager, Warning,
			TEXT("Music Audio Component B is not valid"))
		return nullptr;
	}
	
	return MusicAudioComponentB.Get(); 
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
	if (!IsValid(MusicAudioComponentA)) return;
	
	UMetaSoundOutputSubsystem* OutputSubsystem = GetWorld()->GetSubsystem<UMetaSoundOutputSubsystem>();
	if (!OutputSubsystem)
	{
		UE_LOG(LogTG26MusicManager, Warning, TEXT("MetaSoundOutput Subsystem unavailable"));
		return;
	}
	
	bLoggedTypeMismatch = false;
	
	FOnMetasoundOutputValueChanged HandleEnvelopeDelegate;
	HandleEnvelopeDelegate.BindDynamic(this, &ATG26_MusicManager::HandleEnvelopeChanged);
	
	const bool bSuccessfullyWatching = OutputSubsystem->WatchOutput(MusicAudioComponentA, EnvelopeOutputName, HandleEnvelopeDelegate);
	
	if (!bSuccessfullyWatching)
	{
		UE_LOG(LogTG26MusicManager, Warning,
			TEXT("WatchOutput failed for '%s' - is the MetaSound playing?"), *EnvelopeOutputName.ToString());
	}
}


void ATG26_MusicManager::HandleEnvelopeChanged(FName OutputName, const FMetaSoundOutput& MetaSoundOutput)
{
	float NewValue = 0.f;
	
	if (MetaSoundOutput.Get(NewValue))
	{
		if (FMath::IsNearlyEqual(CurrentEnvelope, NewValue, KINDA_SMALL_NUMBER)) return;
		
		CurrentEnvelope = NormalizingAdjustment * NewValue;
		
		// Broadcast from a MusicManager BP subscribable delegate
		// OnMusicEnvelopeChanged.Broadcast(CurrentEnvelope);
		
		// UPDATE Niagara Parameter Collection
		NPCMusicFXInstance->SetFloatParameter(EnvelopeOutputNameString, CurrentEnvelope);
		
		UNiagaraParameterCollectionInstance* WorldInstance =
			UNiagaraFunctionLibrary::GetNiagaraParameterCollection(this, NPCMusicFX);
		const float ReadBack = NPCMusicFXInstance->GetFloatParameter(EnvelopeOutputNameString);

		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Cyan, FString::Printf(
			TEXT("NPC %s | '%s' | sent %.3f | readback %.3f | same instance: %s"),
			*GetPathNameSafe(NPCMusicFX), *EnvelopeOutputNameString, CurrentEnvelope, ReadBack,
			WorldInstance == NPCMusicFXInstance ? TEXT("yes") : TEXT("no")));
	}
	else
	{
		// Output.Get() FAILS if there is a type mismatch
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
