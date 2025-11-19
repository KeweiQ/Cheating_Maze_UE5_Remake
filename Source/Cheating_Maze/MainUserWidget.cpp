// Fill out your copyright notice in the Description page of Project Settings.

#include "MainUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h" 


void UMainUserWidget::ChangeWidgetVisibilityByName(FName WidgetName, ESlateVisibility NewVisibility)
{
	// Get widget component
	if (UWidget* Widget = GetWidgetFromName(WidgetName))
	{
		Widget->SetVisibility(NewVisibility);
	}

}

void UMainUserWidget::HideAllWidgets()
{
	// Get root widget
	if (UPanelWidget* RootPanel = Cast<UPanelWidget>(GetRootWidget()))
	{
		const int ChildCount = RootPanel->GetChildrenCount();
		
		for (int i = 0; i < ChildCount; ++i)
		{
			// Get child widget
			if (UWidget* Child = RootPanel->GetChildAt(i))
			{
				Child->SetVisibility(ESlateVisibility::Hidden);
			}
		}

	}

}
