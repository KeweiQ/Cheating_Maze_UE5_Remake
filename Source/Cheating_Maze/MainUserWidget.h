#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUserWidget.generated.h"


UCLASS()
class CHEATING_MAZE_API UMainUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// In-game state widget visibility status
	UPROPERTY()
	TArray<UWidget*> VisibleWidgetsInGame;
	
	// Check-object state widget visibility status
	UPROPERTY()
	TArray<UWidget*> VisibleWidgetsCheck;
	
public:
	// Update timer
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UI")
	void UpdateTimer(float TimerVal);

	// Update timer
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UI")
	void UpdateWinTimer();

	// Change text box visibility
	UFUNCTION(BlueprintCallable)
	void ChangeWidgetVisibilityByName(FName WidgetName, ESlateVisibility NewVisibility);

	// Hide all text boxes
	UFUNCTION(BlueprintCallable)
	void HideAllWidgets();

	// Record widget visibility of the current status
	UFUNCTION(BlueprintCallable)
	void RecordWidgetsVisibility();

	// Restore widget visibility of the current status
	UFUNCTION(BlueprintCallable)
	void RestoreWidgetsVisibility();

};
