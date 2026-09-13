#include "UI/Widgets/TitleScreenWidget.h"

#include "Components/Button.h"
#include "GameFlow/BOGameInstance.h"
#include "UI/Manager/UIManager.h"
#include "Kismet/GameplayStatics.h"

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
		UBOGameInstance* GI = Cast<UBOGameInstance>(UGameplayStatics::GetGameInstance(this));
		if (!GI)
			return;
		GI->Start();
	}
}

void UTitleScreenWidget::OnExitButtonClicked()
{
	if (UUIManager* UIManager = UUIManager::Get(this))
	{
		UBOGameInstance* GI = Cast<UBOGameInstance>(UGameplayStatics::GetGameInstance(this));
		if (!GI)
			return;
		GI->Exit();
	}
}
