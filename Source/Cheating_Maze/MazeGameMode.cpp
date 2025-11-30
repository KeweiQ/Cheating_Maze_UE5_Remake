#include "MazeGameMode.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AMazeGameMode::AMazeGameMode()
{

}

// Called when the game starts or when spawned
void AMazeGameMode::BeginPlay()
{
	Super::BeginPlay();

	check(GEngine != nullptr);

}

// Called every frame
void AMazeGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMazeGameMode::StartLevel()
{
	// Update identifier
	bStart = true;

}

void AMazeGameMode::RestartLevel()
{
	if (UWorld* CurrentWorld = GetWorld())
	{
		if (bStart == true && (bPause == true || bWin == true))
		{
			// Reload the current level
			FName CurrentLevelName = *CurrentWorld->GetName();
			UGameplayStatics::OpenLevel(CurrentWorld, CurrentLevelName);
		}
	}

}

void AMazeGameMode::TogglePause()
{
	// Only pause the game when the game has started, not paused yet, and not win yet
	if (bPause == false && bStart == true && bWin == false)
	{
		// Update identifier
		bPause = true;

		// Update game world
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
	// Only unpause the game when it is already paused
	else if (bPause == true)
	{
		// Update identifier
		bPause = false;

		// Update game world
		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}

}

void AMazeGameMode::Win()
{
	// Update identifier
	bWin = true;

}

bool AMazeGameMode::GetWinState() {
	return bWin;

}

bool AMazeGameMode::GetStartState()
{
	return bStart;

}

bool AMazeGameMode::GetPauseState()
{
	return bPause;

}
