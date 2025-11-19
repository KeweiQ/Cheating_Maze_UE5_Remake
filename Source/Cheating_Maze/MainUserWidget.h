// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUserWidget.generated.h"


UCLASS()
class CHEATING_MAZE_API UMainUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
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

};
