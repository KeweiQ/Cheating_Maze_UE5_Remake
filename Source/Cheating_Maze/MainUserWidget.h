#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AudioManager.h"
#include "MainUserWidget.generated.h"


class UWidgetAnimation;


UCLASS()
class CHEATING_MAZE_API UMainUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// Called when the widget is constructed
	virtual void NativeConstruct() override;

	/* ---------- Arrays to record widget visibilities in the last stage ---------- */

	// In-game state widget visibility status
	UPROPERTY()
	TArray<UWidget*> VisibleWidgetsInGame;
	
	// Check-object state widget visibility status
	UPROPERTY()
	TArray<UWidget*> VisibleWidgetsCheck;

	/* ---------- Splash animation at game start ---------- */

	UPROPERTY(meta = (BindWidgetAnimOptional), Transient)
	TObjectPtr<UWidgetAnimation> SplashAnimation;
	
	/* ---------- Game audio ---------- */

	// Audio manager instance
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<AAudioManager> AudioManager;
	
public:
	/* ---------- Timer recording how long to beat the maze ---------- */

	// Update timer
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UI")
	void UpdateTimer(float TimerVal);

	// Update timer
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UI")
	void UpdateWinTimer();

	/* ---------- Functions related to updating widget visibilities ---------- */

	// Change text box visibility
	UFUNCTION(BlueprintCallable)
	void ChangeWidgetVisibilityByName(FName WidgetName, ESlateVisibility NewVisibility);

	// Hide all text boxes
	UFUNCTION(BlueprintCallable)
	void HideAllWidgets(bool bPlayUIAudio);

	// Record widget visibility of the current status
	UFUNCTION(BlueprintCallable)
	void RecordWidgetsVisibility();

	// Restore widget visibility of the current status
	UFUNCTION(BlueprintCallable)
	void RestoreWidgetsVisibility();

	/* ---------- Splash animation at game start ---------- */

	// Play a given animation by name
	UFUNCTION(BlueprintCallable)
	void PlaySplashAnimation();
	
	// Get a given animation' by name's duration
	UFUNCTION(BlueprintCallable)
	float GetSplashAnimationDuration();

};
