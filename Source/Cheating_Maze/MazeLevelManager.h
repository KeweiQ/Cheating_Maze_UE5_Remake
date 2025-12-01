#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMeshActor.h" 
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "MazeLevelManager.generated.h"


class AStaticMeshActor;
class ADirectionalLight;
class ASkyLight;
class AMazePlayerCharacter;
class AMazeGameMode;
class UMainUserWidget;


UCLASS()
class CHEATING_MAZE_API AMazeLevelManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMazeLevelManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* ---------- References to game related objects ---------- */

	// Game Mode
	UPROPERTY()
	TObjectPtr<AMazeGameMode> GameMode;

	// Main UI widget
	UPROPERTY()
	TObjectPtr<UMainUserWidget> MainWidgetInstance;

	// Player character
	UPROPERTY()
	TObjectPtr<AMazePlayerCharacter> PlayerCharacter;

	// Player controller
	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;

	/* ---------- Cheat state identifiers ---------- */

	UPROPERTY()
	bool bLight = false;

	UPROPERTY()
	bool bCamera = false;

	UPROPERTY()
	bool bMap = false;

	UPROPERTY()
	bool bPath = false;

	/* ---------- Cheat controllers ---------- */

	// Toggle the object inspect state
	UFUNCTION()
	void ToggleInspect(FName ObjectName);

	// Toggle the mmap light
	UFUNCTION()
	void ToggleLight();

	// Toggle the player camera (FP or top-down)
	UFUNCTION()
	void ToggleCamera();

	// Toggle the solution path
	UFUNCTION()
	void TogglePath();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	/* ---------- References to cheat related objects ---------- */

	// Solution path actor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cheat")
	TObjectPtr<AActor> Path;

	// Stellar skybox
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cheat")
	TObjectPtr<AStaticMeshActor> StellarSkybox;

	// Space skybox
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cheat")
	TObjectPtr<AStaticMeshActor> SpaceSkybox;

	// Directional light
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cheat")
	TObjectPtr<ADirectionalLight> DirectionalLight;

	// Skylight
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cheat")
	TObjectPtr<ASkyLight> SkyLight;

	// Cheating interactors
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cheat")
	TArray<AActor*> Interactors;
	
	/* ---------- Callable cheat interfaces ---------- */

	// Interact with an interactable
	UFUNCTION()
	void Interact(FString& InteractableType, AActor* Interactor);

	// Cancel all enabled cheatings
	UFUNCTION()
	void CancelCheating();
	
	// Reset the player camera to first-person view
	UFUNCTION()
	void ResetCamera();
	
	/* ---------- Other helper functions ---------- */

	// Let other class set widget instance once it creates it
	UFUNCTION()
	void SetMainWidgetInstance(UMainUserWidget* Instance);

};
