#include "UI/Widgets/KillCountEntryWidget.h"
#include "Components/TextBlock.h"

void UKillCountEntryWidget::SetKillCountEntry(const FText& InMonsterName, int32 InMonsterKillCount)
{
	if (MonsterNameText)
	{
		MonsterNameText->SetText(InMonsterName);
	}

	if (MonsterKillCountText)
	{
		MonsterKillCountText->SetText(FText::AsNumber(InMonsterKillCount));
	}
}
