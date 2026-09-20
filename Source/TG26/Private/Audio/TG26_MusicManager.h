// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TG26_MusicManager.generated.h"

struct FMetaSoundOutput;
enum class EAudioComponentPlayState : uint8;
DECLARE_LOG_CATEGORY_EXTERN(LogTG26MusicManager, Log, All);

UCLASS()
class TG26_API ATG26_MusicManager : public AActor
{
	GENERATED_BODY()

public:
	ATG26_MusicManager();
	

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void HandlePlayStateChanged(EAudioComponentPlayState NewState);
	
	UFUNCTION()
	void HandleEnvelopeChanged(FName OutputName, const FMetaSoundOutput& Output);
	
private:
	void StartWatching();

protected:
	UPROPERTY(VisibleAnywhere, Category="TG26|Audio|Music")
	TObjectPtr<UAudioComponent> MusicAudioComponent;
	
};
