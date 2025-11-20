// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeLevelManager.h"
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

	// Get main widget instance
	MainWidgetInstance = GameMode->MainWidgetInstance;

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

void AMazeLevelManager::Interact(FString& InteractableType)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, InteractableType);
	if (GameMode->bPaused == false && PlayerController && MainWidgetInstance)
	{
		if (InteractableType == TEXT("Hint"))
		{
			ToggleInspect(TEXT("HintImage"));
		}

		else if (InteractableType == TEXT("Light"))
		{
			ToggleLight();
		}

		else if (InteractableType == TEXT("Camera"))
		{
			// Update UI
			MainWidgetInstance->ChangeWidgetVisibilityByName("CheatText", ESlateVisibility::Visible);
			MainWidgetInstance->ChangeWidgetVisibilityByName("ResetText", ESlateVisibility::Visible);

		}

		else if (InteractableType == TEXT("Map"))
		{
			ToggleInspect(TEXT("MapImage"));
		}

		else if (InteractableType == TEXT("Path"))
		{
			TogglePath();

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
			MainWidgetInstance->ChangeWidgetVisibilityByName("CheatText", ESlateVisibility::Visible); //////////////////
		}
		MainWidgetInstance->RecordWidgetsVisibility();
		MainWidgetInstance->HideAllWidgets();
		MainWidgetInstance->ChangeWidgetVisibilityByName("CloseText", ESlateVisibility::Visible);
		MainWidgetInstance->ChangeWidgetVisibilityByName(ObjectName, ESlateVisibility::Visible);
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
		SkyLight->SetActorHiddenInGame(false);

		// Turn off player spot light
		//PlayerCharacter->PlayerSpotLight->SetVisibility(false);

		// Update UI
		MainWidgetInstance->ChangeWidgetVisibilityByName("CheatText", ESlateVisibility::Visible);

	}
	else
	{
		// Change skybox
		StellarSkybox->SetActorHiddenInGame(false);
		SpaceSkybox->SetActorHiddenInGame(true);

		// Turn on map lights
		DirectionalLight->SetActorHiddenInGame(true);
		SkyLight->SetActorHiddenInGame(true);

		// Turn off player spot light
		//PlayerCharacter->PlayerSpotLight->SetVisibility(true);
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
		MainWidgetInstance->ChangeWidgetVisibilityByName("CheatText", ESlateVisibility::Visible);
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

}
