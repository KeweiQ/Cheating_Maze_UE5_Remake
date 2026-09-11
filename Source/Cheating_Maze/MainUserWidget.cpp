#include "MainUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h" 
#include "Animation/WidgetAnimation.h"
#include "Animation/UMGSequencePlayer.h"  
#include <Kismet/GameplayStatics.h>


void UMainUserWidget::ChangeWidgetVisibilityByName(FName WidgetName, ESlateVisibility NewVisibility)
{
	// Get widget component
	if (UWidget* Widget = GetWidgetFromName(WidgetName))
	{
		Widget->SetVisibility(NewVisibility);
	}

}

void UMainUserWidget::HideAllWidgets(bool bPlayUIAudio)
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
				// Set visibility for child widget
				Child->SetVisibility(ESlateVisibility::Hidden);
			}
		}

		// Play ui sound
		if (bPlayUIAudio == true && AudioManager && AudioManager->UIAudio)
		{
			UGameplayStatics::PlaySound2D(this, AudioManager->UIAudio);
		}
	}

}

void UMainUserWidget::RecordWidgetsVisibility()
{
	// Get root widget
	if (UPanelWidget* RootPanel = Cast<UPanelWidget>(GetRootWidget()))
	{
		// Check the state needs to be saved (in-game or check object)
		TArray<UWidget*>& VisibleWidgetsToUse = VisibleWidgetsInGame.IsEmpty() ? VisibleWidgetsInGame : VisibleWidgetsCheck;

		const int ChildCount = RootPanel->GetChildrenCount();

		for (int i = 0; i < ChildCount; ++i)
		{
			// Get child widget
			if (UWidget* Child = RootPanel->GetChildAt(i))
			{
				// Store visible widgets info
				if (Child->GetVisibility() == ESlateVisibility::Visible)
				{
					VisibleWidgetsToUse.Add(Child);
					int32 Length = VisibleWidgetsToUse.Num();
					FString Msg = FString::Printf(TEXT("MyArray length: %d"), Length);
				}
			}
		}

	}

}

void UMainUserWidget::RestoreWidgetsVisibility()
{
	// Get root widget
	if (UPanelWidget* RootPanel = Cast<UPanelWidget>(GetRootWidget()))
	{
		// Check the state needs to be restored (in-game or check object)
		TArray<UWidget*>& VisibleWidgetsToUse = VisibleWidgetsCheck.IsEmpty() ? VisibleWidgetsInGame : VisibleWidgetsCheck;
		
		// Set visiblity for all stored widgets
		for (UWidget* Widget : VisibleWidgetsToUse)
		{
			Widget->SetVisibility(ESlateVisibility::Visible);
		}

		// Clean stored info
		VisibleWidgetsToUse.Empty();
	}

}

void UMainUserWidget::PlaySplashAnimation()
{
	if (SplashAnimation)
	{
		PlayAnimation(SplashAnimation);
	}

}

float UMainUserWidget::GetSplashAnimationDuration()
{
	float duration = 0.0f;

	if (SplashAnimation)
	{
		duration = SplashAnimation->GetEndTime() - SplashAnimation->GetStartTime();
	}

	return duration;
}
