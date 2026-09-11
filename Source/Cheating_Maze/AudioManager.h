#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AudioManager.generated.h"


UCLASS()
class CHEATING_MAZE_API UAudioManager : public UDataAsset
{
	GENERATED_BODY()

public:
	/* --------------- Audio and meta sounds --------------- */

	// UI audio
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundBase* UIAudio;
	
	// console interaction audio
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundBase* ConsoleInteractAudio;
	
};
