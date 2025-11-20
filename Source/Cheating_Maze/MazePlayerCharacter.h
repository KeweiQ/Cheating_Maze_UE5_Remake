// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "MazePlayerCharacter.generated.h"


class AMazeGameMode;
class AMazeLevelManager;
class UInputMappingContext;
class UInputAction;
class UInputComponent;
class UCharacterMovementComponent;


UCLASS()
class CHEATING_MAZE_API AMazePlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMazePlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Game mode
	UPROPERTY()
	TObjectPtr<AMazeGameMode> MazeGameMode;

	// Level manager instance
	UPROPERTY()
	TObjectPtr<AMazeLevelManager> LevelManager;
	
	// Player moving speed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float MaxMoveSpeed = 600.0f;

	// Player turning rate
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float TurnRate = 45.0f;

	// Input mapping context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> MazeGameMappingContext;

	// Input actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> CancelAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> StartAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> RestartAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> TogglePauseAction;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Check if the player can interact with an interactable
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString InteractableType = TEXT("");

	// Let other class set level manager instance
	UFUNCTION()
	void SetLevelManager(AMazeLevelManager* Instance);
	
	// Handles two types of movement
	UFUNCTION()
	void Move(const FInputActionValue& Value);

	// Handles interactions with interactbles
	UFUNCTION()
	void OnInteractPressed(const FInputActionValue& Value);

	// Reset cheating functions
	UFUNCTION()
	void OnCancelPressed(const FInputActionValue& Value);

	// Start the game
	UFUNCTION()
	void OnStartPressed(const FInputActionValue& Value);

	// Restart the game
	UFUNCTION()
	void OnRestartPressed(const FInputActionValue& Value);

	// Pause or resume the game
	UFUNCTION()
	void OnPausePressed(const FInputActionValue& Value);

};
