// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TG26_MusicManager.generated.h"

class UNiagaraParameterCollectionInstance;
class UNiagaraParameterCollection;
struct FMetaSoundOutput;
enum class EAudioComponentPlayState : uint8;

DECLARE_LOG_CATEGORY_EXTERN(LogTG26MusicManager, Log, All);

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMusicEnvelopeChanged, float, EnvelopeVAlue);

UCLASS()
class TG26_API ATG26_MusicManager : public AActor
{
	GENERATED_BODY()

public:
	ATG26_MusicManager();
	
	// UPROPERTY(BlueprintAssignable, Category="TG26|Music")
	// FOnMusicEnvelopeChanged OnMusicEnvelopeChanged;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TG26|Music")
	bool bAutoPlay = false;
	
	// Music Component A has functionality for FX
	UFUNCTION(BlueprintPure, Category="TG26|Music")
	UAudioComponent* GetMusicAudioComponentA() const;
	
	// Music Component B has NO functionality for FX
	UFUNCTION(BlueprintPure, Category="TG26|Music")
	UAudioComponent* GetMusicAudioComponentB() const;
	
	UFUNCTION()
	void HandlePlayStateChanged(EAudioComponentPlayState PlayState);
	
	UFUNCTION()
	void HandleEnvelopeChanged(FName OutputName, const FMetaSoundOutput& Output);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TG26|Music")
	bool bShouldWatchEnvelope = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TG26|Music")
	FName EnvelopeOutputName = TEXT("MX_EnvelopeLead");
	
	FString EnvelopeOutputNameString;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="TG26|Music")
	float NormalizingAdjustment = 3.33f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="TG26|NiagaraParameters")
	TObjectPtr<UNiagaraParameterCollection> NPCMusicFX;
	
	UPROPERTY(Transient)
	TObjectPtr<UNiagaraParameterCollectionInstance> NPCMusicFXInstance;
	
private:
	UFUNCTION()
	void StartWatching();
	
	UPROPERTY()
	float CurrentEnvelope = 0.f;
	
	bool bLoggedTypeMismatch = false;

protected:
	// Music Component A has functionality for FX
	UPROPERTY(VisibleAnywhere, Category="TG26|Audio|Music")
	TObjectPtr<UAudioComponent> MusicAudioComponentA;
	
	// Music Component B has NO functionality for FX
	UPROPERTY(VisibleAnywhere, Category="TG26|Audio|Music")
	TObjectPtr<UAudioComponent> MusicAudioComponentB;
};
