#include "UI/Widgets/TitleScreenWidget.h"

#include "Components/Button.h"
#include "GameFlow/BOGameInstance.h"
#include "UI/Manager/UIManager.h"

void UTitleScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UTitleScreenWidget::OnStartButtonClicked);
	}
}

void UTitleScreenWidget::OnStartButtonClicked()
{
	if (UUIManager* UIManager = UUIManager::Get(this))
	{
		UBOGameInstance* GI = GetWorld() ? Cast<UBOGameInstance>(GetWorld()->GetGameInstance()) : nullptr;
		if (!GI)
			return;
		GI->Start();
	}
}

void UTitleScreenWidget::OnExitButtonClicked()
{
	if (UUIManager* UIManager = UUIManager::Get(this))
	{
		UBOGameInstance* GI = GetWorld() ? Cast<UBOGameInstance>(GetWorld()->GetGameInstance()) : nullptr;
		if (!GI)
			return;
		GI->Exit();
	}
}
