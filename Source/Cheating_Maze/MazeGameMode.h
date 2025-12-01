#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MazeGameMode.generated.h"


class AMazePlayerCharacter;
class AMazeLevelManager;
class UMainUserWidget;
class UGameplayStatics;


UCLASS()
class CHEATING_MAZE_API AMazeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Sets default values
	AMazeGameMode();

protected:
	/* ---------- Game state identifiers ---------- */

	bool bWin = false;

	bool bStart = false;

	bool bPause = false;

	static bool bSplash;

public:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* ---------- Game state change ---------- */

	// Start the game
	UFUNCTION()
	void StartLevel();

	// Restart the game
	UFUNCTION()
	void RestartLevel();

	// Pause or resume the game
	UFUNCTION()
	void TogglePause();

	// Show win message
	UFUNCTION()
	void Win();

	/* ---------- Getters for game state identifiers ---------- */

	UFUNCTION()
	bool GetWinState();

	UFUNCTION()
	bool GetStartState();

	UFUNCTION()
	bool GetPauseState();

	UFUNCTION()
	bool GetSplashState();

	UFUNCTION()
	void SetSplashState(bool NewState);

};
