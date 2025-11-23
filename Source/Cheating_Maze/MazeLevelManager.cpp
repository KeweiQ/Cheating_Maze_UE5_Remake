// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeLevelManager.h"
#include "Camera/CameraComponent.h"
#include "Engine/SkyLight.h"
#include "Components/SkyLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MazeGameMode.h"
#include "MazePlayerCharacter.h"
#include "MainUserWidget.h"


// Sets default values
AMazeLevelManager::AMazeLevelManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMazeLevelManager::BeginPlay()
{
	Super::BeginPlay();

	check(GEngine != nullptr);
	
	// Get game mode
	GameMode = CastChecked<AMazeGameMode>(UGameplayStatics::GetGameMode(this));

	// Get player character
	PlayerCharacter = CastChecked<AMazePlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	// Get player coltroller
	PlayerController = GetWorld()->GetFirstPlayerController();
	check(PlayerController != nullptr);
	
}

// Called every frame
void AMazeLevelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMazeLevelManager::SetMainWidgetInstance(UMainUserWidget* Instance)
{
	MainWidgetInstance = Instance;

}

void AMazeLevelManager::Interact(FString& InteractableType, AActor* Interactor)
{
	if (GameMode->bPaused == false && PlayerController && MainWidgetInstance)
	{
		if (InteractableType == TEXT("Hint"))
		{
			ToggleInspect(TEXT("HintImage"));
		}
		else
		{
			// Call corresponding cheating functions
			if (InteractableType == TEXT("Light"))
			{
				GameMode->bLight = true;
				ToggleLight();
			}
			else if (InteractableType == TEXT("Camera"))
			{
				GameMode->bCamera = true;
				ToggleCamera();
			}
			else if (InteractableType == TEXT("Map"))
			{
				ToggleInspect(TEXT("MapImage"));
			}
			else if (InteractableType == TEXT("Path"))
			{
				GameMode->bPath = true;
				TogglePath();
			}
			
			// Disable cheating interactors
			if (Interactor != nullptr)
			{
				Interactor->SetActorEnableCollision(false);
			}
		}
	}

}

void AMazeLevelManager::ToggleInspect(FName ObjectName)
{
	// From in-game state to object inspect state
	if (!PlayerController->IsMoveInputIgnored())
	{
		// Disable player input
		PlayerController->SetIgnoreMoveInput(true);
		PlayerController->SetIgnoreLookInput(true);

		// Update UI
		if (ObjectName.ToString() == "MapImage")
		{
			MainWidgetInstance->ChangeWidgetVisibilityByName(TEXT("CheatBorder"), ESlateVisibility::Visible);
		}
		MainWidgetInstance->RecordWidgetsVisibility();
		MainWidgetInstance->HideAllWidgets();
		MainWidgetInstance->ChangeWidgetVisibilityByName(TEXT("CloseBorder"), ESlateVisibility::Visible);
		MainWidgetInstance->ChangeWidgetVisibilityByName(ObjectName, ESlateVisibility::Visible);
		if (ObjectName.ToString() == "MapImage")
		{
			MainWidgetInstance->ChangeWidgetVisibilityByName(TEXT("CheatBorder"), ESlateVisibility::Visible);
		}
	}

	// From object inspect state to in-game state
	else
	{
		// Enable player input
		PlayerController->SetIgnoreMoveInput(false);
		PlayerController->SetIgnoreLookInput(false);

		// Update UI
		MainWidgetInstance->HideAllWidgets();
		MainWidgetInstance->RestoreWidgetsVisibility();
	}
}

void AMazeLevelManager::ToggleLight()
{
	if (DirectionalLight->IsHidden())
	{
		// Change skybox
		StellarSkybox->SetActorHiddenInGame(true);
		SpaceSkybox->SetActorHiddenInGame(false);

		// Turn on map lights
		DirectionalLight->SetActorHiddenInGame(false);
		if (USkyLightComponent* SkyLightComponent = Cast<USkyLightComponent>(SkyLight->GetLightComponent()))
		{
			SkyLightComponent->RecaptureSky();
		}
		SkyLight->SetActorHiddenInGame(false);

		// Turn off player spot light
		PlayerCharacter->PlayerSpotLight->SetVisibility(false);

		// Update UI
		MainWidgetInstance->ChangeWidgetVisibilityByName(TEXT("CheatBorder"), ESlateVisibility::Visible);

	}
	else
	{
		// Change skybox
		StellarSkybox->SetActorHiddenInGame(false);
		SpaceSkybox->SetActorHiddenInGame(true);

		// Turn on map lights
		DirectionalLight->SetActorHiddenInGame(true);
		if (USkyLightComponent* SkyLightComponent = Cast<USkyLightComponent>(SkyLight->GetLightComponent()))
		{
			SkyLightComponent->RecaptureSky();
		}
		SkyLight->SetActorHiddenInGame(true);

		// Turn off player spot light
		PlayerCharacter->PlayerSpotLight->SetVisibility(true);
	}

}

void AMazeLevelManager::TogglePath()
{
	if (Path->IsHidden())
	{
		// Update parent actor hidden status
		Path->SetActorHiddenInGame(false);

		// Get attached actors
		TArray<AActor*> Chidren;
		Path->GetAttachedActors(Chidren);

		// Set attached actors hidden status
		for (AActor* Child : Chidren)
		{
			Child->SetActorHiddenInGame(false);
		}

		// Update UI
		MainWidgetInstance->ChangeWidgetVisibilityByName(TEXT("CheatBorder"), ESlateVisibility::Visible);
	}
	else
	{
		// Update parent actor hidden status
		Path->SetActorHiddenInGame(true);

		// Get attached actors
		TArray<AActor*> Chidren;
		Path->GetAttachedActors(Chidren);

		// Set attached actors hidden status
		for (AActor* Child : Chidren)
		{
			Child->SetActorHiddenInGame(true);
		}
	}

}

void AMazeLevelManager::ToggleCamera()
{
	if (PlayerCharacter->FPCamera && PlayerCharacter->TopDownCamera)
	{
		// Toggle camera activation
		const bool bActive = PlayerCharacter->FPCamera->IsActive();
		PlayerCharacter->FPCamera->SetActive(!bActive);
		PlayerCharacter->TopDownCamera->SetActive(bActive);

		// Update UI
		ESlateVisibility NewVisibility = bActive ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
		MainWidgetInstance->ChangeWidgetVisibilityByName(TEXT("ResetBorder"), NewVisibility);
		if (bActive)
		{
			MainWidgetInstance->ChangeWidgetVisibilityByName(TEXT("CheatBorder"), ESlateVisibility::Visible);
		}

		// Rotate player if enabling top-down camera
		if (bActive)
		{
			FVector Dir = FVector::RightVector;
			FRotator Rotator = Dir.Rotation();
			Rotator.Pitch = 0.0f;
			Rotator.Roll = 0.0f;

			// Update controller rotation because player actor follows it
			if (PlayerController)
			{
				PlayerController->SetControlRotation(Rotator);
			}
		}
	}

}

void AMazeLevelManager::CancelCheating()
{
	if (GameMode->bLight == true)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("CANCEL LIGHT"));
		// Update identifier
		GameMode->bLight = false;

		// Reset to first-person camera
		ToggleLight();
	}

	if (GameMode->bCamera == true)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("CANCEL CAMERA"));
		// Update identifier
		GameMode->bCamera = false;

		// Reset to first-person camera
		ToggleCamera();
	}

	if (GameMode->bPath == true)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("CANCEL PATH"));
		// Update identifier
		GameMode->bPath = false;

		// Reset to first-person camera
		TogglePath();
	}

	// Enable cheating interactors
	for (AActor* Interactor : Interactors)
	{
		Interactor->SetActorEnableCollision(true);
	}

	// Update UI
	MainWidgetInstance->ChangeWidgetVisibilityByName(TEXT("CancelBorder"), ESlateVisibility::Hidden);
	MainWidgetInstance->ChangeWidgetVisibilityByName(TEXT("ResetBorder"), ESlateVisibility::Hidden);
	MainWidgetInstance->ChangeWidgetVisibilityByName(TEXT("CheatBorder"), ESlateVisibility::Hidden);
}

void AMazeLevelManager::ResetCamera()
{
	if (GameMode->bCamera == true)
	{
		// Update identifier
		GameMode->bCamera = false;

		// Reset to first-person camera
		ToggleCamera();
		
		// Enable camera cheating interactor
		for (AActor* Interactor : Interactors)
		{
			if (Interactor->ActorHasTag(TEXT("CameraInteractor")))
			{
				Interactor->SetActorEnableCollision(true);
			}
		}
	}

}
