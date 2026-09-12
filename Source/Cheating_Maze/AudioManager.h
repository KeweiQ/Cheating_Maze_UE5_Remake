#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AudioManager.generated.h"


UCLASS()
class CHEATING_MAZE_API AAudioManager : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	/* --------------- Audio and meta sounds --------------- */

	// UI audio
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundBase* UIAudio;
	
	// console interaction audio
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundBase* ConsoleInteractAudio;

	// player movement audio
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundBase* PlayerMovementAudio;
	
	// Background ambience audio actor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<AActor> BackgroundAmbienceActor;

	// Background ambience audio component
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<UAudioComponent> BackgroundAmbience;
	
	// Play UI Audio once
	UFUNCTION()
	void PlayUIAudio(UObject* WorldContextObject);

	// Play background ambience audio
	UFUNCTION()
	void PlayBackgroundAmbience();

	// Stop background ambience audio
	UFUNCTION()
	void StopBackgroundAmbience();
	
};
