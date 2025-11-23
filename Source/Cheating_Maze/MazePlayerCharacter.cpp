// Fill out your copyright notice in the Description page of Project Settings.

#include "MazePlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SpotLightComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MazeGameMode.h"
#include "MazeLevelManager.h"


// Sets default values
AMazePlayerCharacter::AMazePlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

// Called when the game starts or when spawned
void AMazePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(GEngine != nullptr);
	
	// Get current game mode
	GameMode = CastChecked<AMazeGameMode>(UGameplayStatics::GetGameMode(this));

	// Set player move speed
	GetCharacterMovement()->MaxWalkSpeed = MaxMoveSpeed;

	// Get the player controller for this character
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MazeGameMappingContext, 0);
		}
	}

}

// Called every frame
void AMazePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMazePlayerCharacter::SetLevelManager(AMazeLevelManager* Instance)
{
	LevelManager = Instance;
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

		if (GameMode->bCamera == false)
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
		GameMode->StartLevel();
	}

}

// Restart the game
void AMazePlayerCharacter::OnRestartPressed(const FInputActionValue& Value)
{
	if (GameMode)
	{
		GameMode->RestartLevel();
	}

}

// Pause or resume the game
void AMazePlayerCharacter::OnPausePressed(const FInputActionValue& Value)
{
	if (GameMode)
	{
		GameMode->TogglePause();
	}

}
