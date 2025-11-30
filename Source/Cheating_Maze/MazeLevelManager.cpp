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
	if (GameMode->GetPauseState() == false && PlayerController && MainWidgetInstance)
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
				bLight = true;
				ToggleLight();
			}
			else if (InteractableType == TEXT("Camera"))
			{
				bCamera = true;
				ToggleCamera();
			}
			else if (InteractableType == TEXT("Map"))
			{
				ToggleInspect(TEXT("MapImage"));
			}
			else if (InteractableType == TEXT("Path"))
			{
				bPath = true;
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
	if (DirectionalLight)
	{
		bool bIsHidden = DirectionalLight->IsHidden();

		// Toggle skybox hidden status
		StellarSkybox->SetActorHiddenInGame(bIsHidden);
		SpaceSkybox->SetActorHiddenInGame(!bIsHidden);

		// Toggle map light hidden status
		DirectionalLight->SetActorHiddenInGame(!bIsHidden);
		if (USkyLightComponent* SkyLightComponent = Cast<USkyLightComponent>(SkyLight->GetLightComponent()))
		{
			SkyLightComponent->RecaptureSky();
		}
		SkyLight->SetActorHiddenInGame(!bIsHidden);

		// Toggle player spot light status
		PlayerCharacter->PlayerSpotLight->SetVisibility(!bIsHidden);

		// Update UI
		if (bIsHidden)
		{
			MainWidgetInstance->ChangeWidgetVisibilityByName(TEXT("CheatBorder"), ESlateVisibility::Visible);
		}
	}

}

void AMazeLevelManager::TogglePath()
{
	if (Path)
	{
		bool bIsHidden = Path->IsHidden();

		// Toggle parent actor hidden status
		Path->SetActorHiddenInGame(!bIsHidden);

		// Get attached actors
		TArray<AActor*> Chidren;
		Path->GetAttachedActors(Chidren);

		// Toggle attached actors hidden status
		for (AActor* Child : Chidren)
		{
			Child->SetActorHiddenInGame(!bIsHidden);
		}

		// Update UI
		if (bIsHidden)
		{
			MainWidgetInstance->ChangeWidgetVisibilityByName(TEXT("CheatBorder"), ESlateVisibility::Visible);
		}
	}

}

void AMazeLevelManager::ToggleCamera()
{
	if (PlayerCharacter->FPCamera && PlayerCharacter->TopDownCamera)
	{
		// Toggle camera activation
		const bool bIsActive = PlayerCharacter->FPCamera->IsActive();
		PlayerCharacter->FPCamera->SetActive(!bIsActive);
		PlayerCharacter->TopDownCamera->SetActive(bIsActive);

		// Update UI
		ESlateVisibility NewVisibility = bIsActive ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
		MainWidgetInstance->ChangeWidgetVisibilityByName(TEXT("ResetBorder"), NewVisibility);
		if (bIsActive)
		{
			MainWidgetInstance->ChangeWidgetVisibilityByName(TEXT("CheatBorder"), ESlateVisibility::Visible);
		}

		// Rotate player if enabling top-down camera
		if (bIsActive)
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
	if (bLight == true)
	{
		// Update identifier
		bLight = false;

		// Reset to first-person camera
		ToggleLight();
	}

	if (bCamera == true)
	{
		// Update identifier
		bCamera = false;

		// Reset to first-person camera
		ToggleCamera();
	}

	if (bPath == true)
	{
		// Update identifier
		bPath = false;

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
	if (bCamera == true)
	{
		// Update identifier
		bCamera = false;

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
