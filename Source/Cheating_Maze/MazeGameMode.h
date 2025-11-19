// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainUserWidget.h"
#include "TimerManager.h"
#include "MazeGameMode.generated.h"


class UMainUserWidget;


UCLASS()
class CHEATING_MAZE_API AMazeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Sets default values
	AMazeGameMode();

protected:
	FTimerHandle MazeTimerHandle;

	// Player controller
	TObjectPtr<APlayerController> Controller;
	
	// Update the maze timer
	UFUNCTION()
	void UpdateTimer();

public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY()
	bool bWin = false;

	UPROPERTY()
	bool bStarted = false;

	UPROPERTY()
	bool bPaused = false;

	UPROPERTY()
	bool bCheating = false;

	UPROPERTY()
	bool bTopDownCamera = false;

	UPROPERTY()
	float TimerVal = 0.0f;

	// Is timer on
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	bool TimerStatus = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UMainUserWidget> MainWidgetClass;

	UPROPERTY()
	TObjectPtr<UMainUserWidget> MainWidgetInstance;

	// Start maze timer when entering the maze
	UFUNCTION(BlueprintCallable, Category = "Timer")
	void StartTimer();

	// End maze timer when reaching the exit
	UFUNCTION(BlueprintCallable, Category = "Timer")
	void EndTimer();

	// Pause maze timer when pausing the game
	UFUNCTION(BlueprintCallable, Category = "Timer")
	void PauseTimer();

	// Resume maze timer when resuming the game
	UFUNCTION(BlueprintCallable, Category = "Timer")
	void ResumeTimer();

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
	
};
