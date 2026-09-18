#include "UI/Widgets/KillCountEntryWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "DataTables/Monster/MonsterInfo.h"

void UKillCountEntryWidget::SetKillCountEntry(const FText& InMonsterName, int32 InMonsterKillCount)
{
	if (MonsterImage && MonsterDataTable)
	{
		TArray<FMonsterInfo*> AllRows;
		MonsterDataTable->GetAllRows<FMonsterInfo>(TEXT("KillCountEntry_FindByName"), AllRows);

		for (const FMonsterInfo* Row : AllRows)
		{
			if (Row && Row->MonsterName.ToString() == InMonsterName.ToString())
			{
				if (Row->MonsterImage)
				{
					MonsterImage->SetBrushFromTexture(Row->MonsterImage);
				}
				break;
			}
		}
	}

	if (MonsterNameText)
	{
		MonsterNameText->SetText(InMonsterName);
	}

	if (MonsterKillCountText)
	{
		MonsterKillCountText->SetText(FText::AsNumber(InMonsterKillCount));
	}
}
