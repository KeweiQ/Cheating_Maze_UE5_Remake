#include "AudioManager.h"
#include "Components/AudioComponent.h"
#include <Kismet/GameplayStatics.h>


void AAudioManager::BeginPlay()
{
	Super::BeginPlay();
	
	// Get audio component form actor
	if (BackgroundAmbienceActor)
	{
		BackgroundAmbience = BackgroundAmbienceActor->FindComponentByClass<UAudioComponent>();
	}
}

void AAudioManager::PlayUIAudio(UObject* WorldContextObject)
{
	if (UIAudio && WorldContextObject)
	{
		UGameplayStatics::PlaySound2D(WorldContextObject, UIAudio);
	}
}

void AAudioManager::PlayBackgroundAmbience()
{
	if (BackgroundAmbience && !BackgroundAmbience->IsPlaying())
	{
		BackgroundAmbience->Play();
	}

}

void AAudioManager::StopBackgroundAmbience()
{
	if (BackgroundAmbience && BackgroundAmbience->IsPlaying())
	{
		BackgroundAmbience->Stop();
	}

}
