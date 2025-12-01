#pragma once

#include "CoreMinimal.h"
#include "TimerManager.h"
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
class USpotLightComponent;
class UCameraComponent;
class UCharacterMovementComponent;
class UMainUserWidget;


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

	/* ---------- References to game related objects ---------- */

	// Game mode
	UPROPERTY()
	TObjectPtr<AMazeGameMode> GameMode;

	// Level manager instance
	UPROPERTY()
	TObjectPtr<AMazeLevelManager> LevelManager;

	/* --------------- Setup related functions --------------- */

	// Generate components to setup player
	UFUNCTION()
	void SetupPlayer();

	// Cast and setup player controller
	UFUNCTION()
	void SetupController();

	// Generate widget components to setup UI
	UFUNCTION()
	void SetupUI();

	/* --------------- Movement related variables and functions --------------- */

	// Player moving speed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float MaxMoveSpeed = 600.0f;

	// Player turning rate
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float TurnRate = 60.0f;

	// Handles two types of movement
	UFUNCTION()
	void Move(const FInputActionValue& Value);

	/* --------------- Inteactor type detection variables --------------- */

	// Check if the player can interact with an interactable
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString InteractableType = TEXT("");

	// Check if the player can interact with an interactable
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> Interactor = nullptr;

	/* --------------- References to input actions --------------- */

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
	TObjectPtr<UInputAction> ResetAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> StartAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> RestartAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> TogglePauseAction;
	
	/* --------------- Input action response functions --------------- */

	// Handles interactions with interactbles
	UFUNCTION()
	void OnInteractPressed(const FInputActionValue& Value);

	// Cancel all enabled cheatings
	UFUNCTION()
	void OnCancelPressed(const FInputActionValue& Value);

	// Reset camera to first-person view
	UFUNCTION()
	void OnResetPressed(const FInputActionValue& Value);

	// Start the game
	UFUNCTION()
	void OnStartPressed(const FInputActionValue& Value);

	// Restart the game
	UFUNCTION()
	void OnRestartPressed(const FInputActionValue& Value);

	// Pause or resume the game
	UFUNCTION()
	void OnPausePressed(const FInputActionValue& Value);

	/* --------------- Timer related variables --------------- */

	UPROPERTY()
	float MazeTimerVal = 0.0f;

	// Timer handle
	UPROPERTY()
	FTimerHandle MazeTimerHandle;

	// Timer handle
	UPROPERTY()
	FTimerHandle SplashTimerHandle;

	/* --------------- Timer control functions --------------- */

	// Start maze timer when entering the maze
	UFUNCTION(BlueprintCallable, Category = "Timer")
	void StartTimer();

	// Update the maze timer
	UFUNCTION()
	void UpdateTimer();

	// End maze timer when reaching the exit
	UFUNCTION(BlueprintCallable, Category = "Timer")
	void EndTimer();

	// Pause maze timer when pausing the game
	UFUNCTION(BlueprintCallable, Category = "Timer")
	void PauseTimer();

	// Resume maze timer when resuming the game
	UFUNCTION(BlueprintCallable, Category = "Timer")
	void ResumeTimer();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* ---------- UI related references ---------- */

	// Widget class to be used to create an instance
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UMainUserWidget> MainWidgetClass;

	// Main UI widget
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UMainUserWidget> MainWidgetInstance;

	/* --------------- Timer related variables accessable by blueprints --------------- */

	// Is timer on
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	bool bTimer = false;

	/* --------------- Player actor components --------------- */

	// Player controller
	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;
	
	// Player mesh component
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> PlayerMesh;

	// Player spot light ccomponent
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USpotLightComponent> PlayerSpotLight;

	// First-person camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> FPCamera;

	// Top-down camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> TopDownCamera;
	

};
