// Fill out your copyright notice in the Description page of Project Settings.

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
	
	if (Controller = GetWorld()->GetFirstPlayerController())
	{
		// Setup UI
		if (MainWidgetClass)
		{
			// Create UI widget instance
			MainWidgetInstance = CreateWidget<UMainUserWidget>(Controller, MainWidgetClass);

			if (MainWidgetInstance)
			{
				// Add widget to viewport
				MainWidgetInstance->AddToViewport();

				// Show initial timer value
				MainWidgetInstance->UpdateTimer(TimerVal);
			}
		}

		// Hide and lock mouse crusor
		Controller->bShowMouseCursor = false;
		Controller->SetInputMode(FInputModeGameOnly());

		// Pause the game at beginning
		GetWorldTimerManager().SetTimerForNextTick(this, &AMazeGameMode::TogglePause);
	}

}

// Called every frame
void AMazeGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMazeGameMode::StartTimer()
{
	// Check if the maze timer is already activated
	if (GetWorldTimerManager().IsTimerActive(MazeTimerHandle))
	{
		return;
	}

	// Activate the maze timer
	GetWorldTimerManager().SetTimer(MazeTimerHandle, this, &AMazeGameMode::UpdateTimer, 1.0f, true);

}

void AMazeGameMode::UpdateTimer()
{
	TimerVal += 1.0f;
	
	// Update timer value on widget
	if (MainWidgetInstance)
	{
		MainWidgetInstance->UpdateTimer(TimerVal);
	}

}

void AMazeGameMode::EndTimer()
{
	GetWorldTimerManager().ClearTimer(MazeTimerHandle);
	Win();

}

void AMazeGameMode::PauseTimer()
{
	GetWorldTimerManager().PauseTimer(MazeTimerHandle);

}

void AMazeGameMode::ResumeTimer()
{
	GetWorldTimerManager().UnPauseTimer(MazeTimerHandle);

}

void AMazeGameMode::StartLevel()
{
	// Update identifier
	bStarted = true;

	TogglePause();

}

void AMazeGameMode::RestartLevel()
{
	if (UWorld* CurrentWorld = GetWorld())
	{
		if (bStarted == true && bPaused == true)
		{
			// Reload the current level
			FName CurrentLevelName = *CurrentWorld->GetName();
			UGameplayStatics::OpenLevel(CurrentWorld, CurrentLevelName);
		}
	}

}

void AMazeGameMode::TogglePause()
{
	if (bPaused == true && bStarted == true && bWin == false) {
		// Update identifier
		bPaused = false;

		// Update game world
		UGameplayStatics::SetGamePaused(GetWorld(), false);

		// Update UI
		MainWidgetInstance->HideAllWidgets();
		MainWidgetInstance->ChangeWidgetVisibilityByName(TEXT("TimerText"), ESlateVisibility::Visible);

		if (bCheating)
		{
			MainWidgetInstance->ChangeWidgetVisibilityByName(TEXT("CheatText"), ESlateVisibility::Visible);
		}

		if (bTopDownCamera)
		{
			MainWidgetInstance->ChangeWidgetVisibilityByName(TEXT("ResetText"), ESlateVisibility::Visible);
		}

	}
	else if (bPaused == false)
	{
		// Update identifier
		bPaused = true;

		// Update game world
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		
		// Update UI
		MainWidgetInstance->HideAllWidgets();

		if (bStarted == false)
		{
			MainWidgetInstance->ChangeWidgetVisibilityByName(TEXT("StartText"), ESlateVisibility::Visible);
		}
		else if (bWin == true)
		{
			MainWidgetInstance->ChangeWidgetVisibilityByName(TEXT("WinOverlay"), ESlateVisibility::Visible);
		}
		else
		{
			MainWidgetInstance->ChangeWidgetVisibilityByName(TEXT("PauseText"), ESlateVisibility::Visible);
		}
	}

}

void AMazeGameMode::Win()
{
	// Update identifier
	bWin = true;

	// Update UI
	if (MainWidgetInstance)
	{
		MainWidgetInstance->UpdateWinTimer();
	}

	TogglePause();

}
