// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TG26_MusicManager.generated.h"

struct FMetaSoundOutput;
enum class EAudioComponentPlayState : uint8;

DECLARE_LOG_CATEGORY_EXTERN(LogTG26MusicManager, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMusicEnvelopeChanged, float, EnvelopeVAlue);

UCLASS()
class TG26_API ATG26_MusicManager : public AActor
{
	GENERATED_BODY()

public:
	ATG26_MusicManager();
	
	UPROPERTY(BlueprintAssignable, Category="TG26|Music")
	FOnMusicEnvelopeChanged OnMusicEnvelopeChanged;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TG26|Music")
	bool bAutoPlay = true;
	
	UFUNCTION()
	void HandlePlayStateChanged(EAudioComponentPlayState PlayState);
	
	UFUNCTION()
	void HandleEnvelopeChanged(FName OutputName, const FMetaSoundOutput& Output);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TG26|Music")
	bool bShouldWatchEnvelope = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TG26|Music")
	FName EnvelopeOutputName = TEXT("EnvelopeLEAD");
	
private:
	UFUNCTION()
	void StartWatching();
	
	UPROPERTY()
	float CurrentEnvelope = 0.f;
	
	bool bLoggedTypeMismatch = false;

protected:
	UPROPERTY(VisibleAnywhere, Category="TG26|Audio|Music")
	TObjectPtr<UAudioComponent> MusicAudioComponent;
	
};
