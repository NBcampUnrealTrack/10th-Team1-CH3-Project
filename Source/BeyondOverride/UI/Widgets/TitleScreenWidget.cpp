#include "UI/Widgets/TitleScreenWidget.h"

#include "Components/Button.h"
#include "GameFlow/BOGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Manager/UIManager.h"

void UTitleScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartBtn)
	{
		StartBtn->OnClicked.AddDynamic(this, &UTitleScreenWidget::OnStartButtonClicked);
	}

	if (ExitBtn)
	{
		ExitBtn->OnClicked.AddDynamic(this, &UTitleScreenWidget::OnExitButtonClicked);
	}
}

void UTitleScreenWidget::OnStartButtonClicked()
{
	if (UUIManager* UIManager = UUIManager::Get(this))
	{
		ABOGameMode* GM = Cast<ABOGameMode>(UGameplayStatics::GetGameMode(this));
		if (!GM)
			return;
		GM->Start();
	}
}

void UTitleScreenWidget::OnExitButtonClicked()
{
	if (UUIManager* UIManager = UUIManager::Get(this))
	{
		ABOGameMode* GM = Cast<ABOGameMode>(UGameplayStatics::GetGameMode(this));
		if (!GM)
			return;
		GM->Exit();
	}
}
