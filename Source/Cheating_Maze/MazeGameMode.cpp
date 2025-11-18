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
	
	if (APlayerController* Controller = GetWorld()->GetFirstPlayerController())
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
	MainWidgetInstance->UpdateTimer(TimerVal);

}

void AMazeGameMode::EndTimer()
{
	GetWorldTimerManager().ClearTimer(MazeTimerHandle);

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
	bPaused = false;
	UGameplayStatics::SetGamePaused(GetWorld(), false);

}

void AMazeGameMode::RestartLevel()
{
	if (UWorld* CurrentWorld = GetWorld())
	{
		// Reload the current level
		FName CurrentLevelName = *CurrentWorld->GetName();
		UGameplayStatics::OpenLevel(CurrentWorld, CurrentLevelName);
	}

}

void AMazeGameMode::TogglePause()
{
	if (bPaused == true) {
		bPaused = false;
		UGameplayStatics::SetGamePaused(GetWorld(), false);

	}
	else
	{
		bPaused = true;
		UGameplayStatics::SetGamePaused(GetWorld(), true);

	}

}
