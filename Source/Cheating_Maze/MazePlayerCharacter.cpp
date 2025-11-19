// Fill out your copyright notice in the Description page of Project Settings.

#include "MazePlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AMazePlayerCharacter::AMazePlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMazePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(GEngine != nullptr);

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

	// Get current game mode
	MazeGameMode = CastChecked<AMazeGameMode>(UGameplayStatics::GetGameMode(this));
	
}

// Called every frame
void AMazePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMazePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind Movement Actions
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMazePlayerCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &AMazePlayerCharacter::OnInteractPressed);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &AMazePlayerCharacter::OnCancelPressed);
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

		// Add left and right rotation
		AddControllerYawInput(MovementValue.X * TurnRate * GetWorld()->GetDeltaSeconds());
	}

}

// Handles interactions with interactbles
void AMazePlayerCharacter::OnInteractPressed(const FInputActionValue& Value)
{
	if (MazeGameMode)
	{

	}

}

// Reset cheating functions
void AMazePlayerCharacter::OnCancelPressed(const FInputActionValue& Value)
{
	if (MazeGameMode)
	{

	}

}

// Start the game
void AMazePlayerCharacter::OnStartPressed(const FInputActionValue& Value)
{
	if (MazeGameMode)
	{
		MazeGameMode->StartLevel();
	}

}

// Restart the game
void AMazePlayerCharacter::OnRestartPressed(const FInputActionValue& Value)
{
	if (MazeGameMode)
	{
		MazeGameMode->RestartLevel();
	}

}

// Pause or resume the game
void AMazePlayerCharacter::OnPausePressed(const FInputActionValue& Value)
{
	if (MazeGameMode)
	{
		MazeGameMode->TogglePause();
	}

}
