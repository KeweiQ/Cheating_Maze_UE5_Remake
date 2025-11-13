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

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using MazePlayerCharacter."));
	
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
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &AMazePlayerCharacter::Interact);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &AMazePlayerCharacter::Cancel);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &AMazePlayerCharacter::StartLevel);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &AMazePlayerCharacter::RestartLevel);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &AMazePlayerCharacter::TogglePause);
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
void AMazePlayerCharacter::Interact(const FInputActionValue& Value)
{

}

// Reset cheating functions
void AMazePlayerCharacter::Cancel(const FInputActionValue& Value)
{

}

// Start the game
void AMazePlayerCharacter::StartLevel(const FInputActionValue& Value)
{

}

// Restart the game
void AMazePlayerCharacter::RestartLevel(const FInputActionValue& Value)
{
	//if (UWorld* CurrentWorld = GetWorld())
	//{
	//	// Reload the current level
	//	FName CurrentLevelName = *CurrentWorld->GetName();
	//	UGameplayStatics::OpenLevel(CurrentWorld, CurrentLevelName);
	//}

}

// Pause or resume the game
void AMazePlayerCharacter::TogglePause(const FInputActionValue& Value)
{

}
