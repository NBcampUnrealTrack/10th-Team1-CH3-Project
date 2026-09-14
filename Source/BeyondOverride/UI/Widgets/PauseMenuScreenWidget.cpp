#include "UI/Widgets/PauseMenuScreenWidget.h"

#include "Components/Button.h"
#include "GameFlow/BOGameInstance.h"
#include "Player/PlayerController/BOPlayerController.h"
#include "UI/Manager/UIManager.h"

void UPauseMenuScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsFocusable(true);

	if (ResumeBtn)
	{
		ResumeBtn->OnClicked.AddDynamic(this, &UPauseMenuScreenWidget::OnResumeButtonClicked);
	}

	if (ExitBtn)
	{
		ExitBtn->OnClicked.AddDynamic(this, &UPauseMenuScreenWidget::OnExitButtonClicked);
	}
}

FReply UPauseMenuScreenWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Escape)
	{
		OnResumeButtonClicked();
		// return FReply::Handled();
	}

	return FReply::Handled();
}

void UPauseMenuScreenWidget::OnResumeButtonClicked()
{
	if (UUIManager* UIManager = UUIManager::Get(this))
	{
		UIManager->PopScreen();
	}
}

void UPauseMenuScreenWidget::OnExitButtonClicked()
{
	if (UBOGameInstance* GI = Cast<UBOGameInstance>(GetWorld()->GetGameInstance()))
	{
		GI->Exit();
	}
}
