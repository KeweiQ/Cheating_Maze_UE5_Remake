#include "MazePlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SpotLightComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MazeGameMode.h"
#include "MazeLevelManager.h"
#include "MainUserWidget.h"


// Sets default values
AMazePlayerCharacter::AMazePlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetupPlayer();

}

// Called when the game starts or when spawned
void AMazePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(GEngine != nullptr);
	
	// Get current game mode
	GameMode = CastChecked<AMazeGameMode>(UGameplayStatics::GetGameMode(this));

	// Get level manager
	LevelManager = CastChecked<AMazeLevelManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AMazeLevelManager::StaticClass()));

	// Set player move speed
	GetCharacterMovement()->MaxWalkSpeed = MaxMoveSpeed;
	
	// Acquire and setup player controller
	SetupController();

	// Create and setup player HUD
	SetupUI();
}

// Called every frame
void AMazePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMazePlayerCharacter::SetupPlayer()
{
	// Get player root capsule component
	UCapsuleComponent* Capsule = GetCapsuleComponent();

	// Create player components
	PlayerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
	check(PlayerMesh != nullptr);
	PlayerSpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("PlayerSpotLight"));
	check(PlayerSpotLight != nullptr);
	FPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPCamera"));
	check(FPCamera != nullptr);
	TopDownCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	check(TopDownCamera != nullptr);

	// Attach the new components
	PlayerMesh->SetupAttachment(Capsule);
	PlayerSpotLight->SetupAttachment(PlayerMesh);
	FPCamera->SetupAttachment(PlayerMesh);
	TopDownCamera->SetupAttachment(PlayerMesh);

}

void AMazePlayerCharacter::SetupController()
{
	// Get the player controller for this character
	if (PlayerController = CastChecked<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MazeGameMappingContext, 0);
		}
		
		// Hide and lock mouse crusor
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetInputMode(FInputModeGameOnly());

		// Disable player control at beginning
		PlayerController->SetIgnoreMoveInput(true);
		PlayerController->SetIgnoreLookInput(true);
	}

}

void AMazePlayerCharacter::SetupUI()
{
	// Get player controller
	if (PlayerController)
	{
		// Setup UI
		if (MainWidgetClass)
		{
			// Create UI widget instance
			MainWidgetInstance = CreateWidget<UMainUserWidget>(PlayerController, MainWidgetClass);

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

				// Send widget instance to level manager
				if (LevelManager)
				{
					LevelManager->SetMainWidgetInstance(MainWidgetInstance);
				}
			}
		}
	}

}

// Called to bind functionality to input
void AMazePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind Movement Actions
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMazePlayerCharacter::Move);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AMazePlayerCharacter::OnInteractPressed);
		EnhancedInputComponent->BindAction(CancelAction, ETriggerEvent::Started, this, &AMazePlayerCharacter::OnCancelPressed);
		EnhancedInputComponent->BindAction(ResetAction, ETriggerEvent::Started, this, &AMazePlayerCharacter::OnResetPressed);
		EnhancedInputComponent->BindAction(StartAction, ETriggerEvent::Started, this, &AMazePlayerCharacter::OnStartPressed);
		EnhancedInputComponent->BindAction(RestartAction, ETriggerEvent::Started, this, &AMazePlayerCharacter::OnRestartPressed);
		EnhancedInputComponent->BindAction(TogglePauseAction, ETriggerEvent::Started, this, &AMazePlayerCharacter::OnPausePressed);
	}

}

// Handles two types of movement
void AMazePlayerCharacter::Move(const FInputActionValue& Value)
{
	// 2D Vector of movement values returned from the input action
	const FVector2D MovementValue = Value.Get<FVector2D>();

	// Check if the controller possessing this Actor is valid
	if (Controller)
	{
		// Add forward and back movement
		AddMovementInput(GetActorForwardVector(), MovementValue.Y);

		if (FPCamera->IsActive() == true)
		{
			// Add left and right rotation
			AddControllerYawInput(MovementValue.X * TurnRate * GetWorld()->GetDeltaSeconds());
		}
		else
		{
			// Add left and right movement
			AddMovementInput(GetActorRightVector(), MovementValue.X); ///////////////////////////
		}
	}

}

// Handles interactions with interactbles
void AMazePlayerCharacter::OnInteractPressed(const FInputActionValue& Value)
{
	if (InteractableType != TEXT(""))
	{
		if (LevelManager)
		{
			LevelManager->Interact(InteractableType, Interactor);
		}
	}

}

// Cancel all enabled cheatings
void AMazePlayerCharacter::OnCancelPressed(const FInputActionValue& Value)
{
	if (LevelManager)
	{
		LevelManager->CancelCheating();
	}

}

// Reset camera to first-person view
void AMazePlayerCharacter::OnResetPressed(const FInputActionValue& Value)
{
	if (LevelManager)
	{
		LevelManager->ResetCamera();
	}

}

// Start the game
void AMazePlayerCharacter::OnStartPressed(const FInputActionValue& Value)
{
	if (GameMode)
	{
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
		
		// Update game state
		GameMode->StartLevel();
	}

}

// Restart the game
void AMazePlayerCharacter::OnRestartPressed(const FInputActionValue& Value)
{
	if (GameMode)
	{
		// Reload current level
		GameMode->RestartLevel();
	}

}

// Pause or resume the game
void AMazePlayerCharacter::OnPausePressed(const FInputActionValue& Value)
{
	if (GameMode)
	{
		// Update UI
		if (GameMode->GetPauseState() == false && GameMode->GetStartState() == true && GameMode->GetWinState() == false)
		{
			if (MainWidgetInstance)
			{
				MainWidgetInstance->RecordWidgetsVisibility();
				MainWidgetInstance->HideAllWidgets();
				MainWidgetInstance->ChangeWidgetVisibilityByName(TEXT("PauseBorder"), ESlateVisibility::Visible);
			}
		}
		else if (GameMode->GetPauseState() == true)
		{
			if (MainWidgetInstance)
			{
				MainWidgetInstance->HideAllWidgets();
				MainWidgetInstance->RestoreWidgetsVisibility();
			}
		}
		
		// Update game state and change game behavior
		GameMode->TogglePause();
	}

}

void AMazePlayerCharacter::StartTimer()
{
	// Check if the maze timer is already activated
	if (GetWorldTimerManager().IsTimerActive(MazeTimerHandle))
	{
		return;
	}

	// Activate the maze timer
	GetWorldTimerManager().SetTimer(MazeTimerHandle, this, &AMazePlayerCharacter::UpdateTimer, 1.0f, true);

}

void AMazePlayerCharacter::UpdateTimer()
{
	TimerVal += 1.0f;

	// Update timer value on widget
	if (MainWidgetInstance)
	{
		MainWidgetInstance->UpdateTimer(TimerVal);
	}

}

void AMazePlayerCharacter::EndTimer()
{
	GetWorldTimerManager().ClearTimer(MazeTimerHandle);

	// Set game state to win
	if (GameMode)
	{
		GameMode->Win();
	}

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

void AMazePlayerCharacter::PauseTimer()
{
	GetWorldTimerManager().PauseTimer(MazeTimerHandle);

}

void AMazePlayerCharacter::ResumeTimer()
{
	GetWorldTimerManager().UnPauseTimer(MazeTimerHandle);

}
