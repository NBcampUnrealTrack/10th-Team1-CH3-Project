#include "UI/Widgets/ResultScreenWidget.h"

#include "UI/Manager/UIManager.h"

void UResultScreenWidget::OnOKBtnClicked()
{
	if (UUIManager* UIManager = UUIManager::Get(this))
	{
		UIManager->PopScreen();
	}
}
