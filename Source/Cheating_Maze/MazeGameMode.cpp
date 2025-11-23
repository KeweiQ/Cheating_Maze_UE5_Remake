// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h" 
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "MainUserWidget.h"
#include "MazeLevelManager.h"
#include "MazePlayerCharacter.h"


// Sets default values
AMazeGameMode::AMazeGameMode()
{

}

// Called when the game starts or when spawned
void AMazeGameMode::BeginPlay()
{
	Super::BeginPlay();

	check(GEngine != nullptr);

	// Get level manager
	LevelManager = CastChecked<AMazeLevelManager>(UGameplayStatics::GetActorOfClass(this, AMazeLevelManager::StaticClass()));
	check(LevelManager != nullptr);
	
	// Get player controller
	if (PlayerController = GetWorld()->GetFirstPlayerController())
	{
		// Get player character
		PlayerCharacter = CastChecked<AMazePlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		
		// Set other class's instance variables
		PlayerCharacter->SetLevelManager(LevelManager);

		// Setup UI
		if (MainWidgetClass)
		{
			// Create UI widget instance
			MainWidgetInstance = CreateWidget<UMainUserWidget>(PlayerController, MainWidgetClass);

			// Set other class's instance variables
			LevelManager->SetMainWidgetInstance(MainWidgetInstance);

			if (MainWidgetInstance)
			{
				// Add widget to viewport
				MainWidgetInstance->AddToViewport();

				// Show initial timer value
				MainWidgetInstance->UpdateTimer(TimerVal);

				// Setup initial UI
				MainWidgetInstance->RecordWidgetsVisibility();
				MainWidgetInstance->HideAllWidgets();
				MainWidgetInstance->ChangeWidgetVisibilityByName(TEXT("StartBorder"), ESlateVisibility::Visible);
			}
		}

		// Hide and lock mouse crusor
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetInputMode(FInputModeGameOnly());

		// Disable player control at beginning
		PlayerController->SetIgnoreMoveInput(true);
		PlayerController->SetIgnoreLookInput(true);
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

	// Update UI
	if (MainWidgetInstance)
	{
		MainWidgetInstance->HideAllWidgets();
		MainWidgetInstance->ChangeWidgetVisibilityByName(TEXT("TimerBorder"), ESlateVisibility::Visible);
	}
	
	// Enable player control
	if (PlayerController)
	{
		PlayerController->SetIgnoreMoveInput(false);
		PlayerController->SetIgnoreLookInput(false);
	}

}

void AMazeGameMode::RestartLevel()
{
	if (UWorld* CurrentWorld = GetWorld())
	{
		if (bStarted == true && (bPaused == true || bWin == true))
		{
			// Reload the current level
			FName CurrentLevelName = *CurrentWorld->GetName();
			UGameplayStatics::OpenLevel(CurrentWorld, CurrentLevelName);
		}
	}

}

void AMazeGameMode::TogglePause()
{
	if (bPaused == false && bStarted == true && bWin == false) {
		// Update identifier
		bPaused = true;

		// Update game world
		UGameplayStatics::SetGamePaused(GetWorld(), true);

		// Update UI
		if (MainWidgetInstance)
		{
			MainWidgetInstance->RecordWidgetsVisibility();
			MainWidgetInstance->HideAllWidgets();
			MainWidgetInstance->ChangeWidgetVisibilityByName(TEXT("PauseBorder"), ESlateVisibility::Visible);
		}
	}

	else if (bPaused == true)
	{
		// Update identifier
		bPaused = false;

		// Update game world
		UGameplayStatics::SetGamePaused(GetWorld(), false);

		// Update UI
		MainWidgetInstance->HideAllWidgets();
		MainWidgetInstance->RestoreWidgetsVisibility();
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
		MainWidgetInstance->HideAllWidgets();
		MainWidgetInstance->ChangeWidgetVisibilityByName(TEXT("WinBorder"), ESlateVisibility::Visible);
	}
	
	// Disable player control
	PlayerController->SetIgnoreMoveInput(true);
	PlayerController->SetIgnoreLookInput(true);

}
