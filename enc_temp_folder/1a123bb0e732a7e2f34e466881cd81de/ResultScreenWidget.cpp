#include "UI/Widgets/ResultScreenWidget.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "GameFlow/BOGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ActorComponent/PlayerInventoryComponent.h"
#include "Player/Character/BOCharacter.h"
#include "UI/Manager/UIManager.h"
#include "UI/Widgets/ItemSlotPanelWidget.h"
#include "UI/Widgets/KillCountEntryWidget.h"

void UResultScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UBOGameInstance* GI = Cast<UBOGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (!GI)
		return;

	ABOCharacter* OwnerCharacter = Cast<ABOCharacter>(GetOwningPlayerPawn());
	if (!OwnerCharacter)
		return;

	bIsSurvived = (GI->GetFarmingResult() == EFarmingResult::Success);

	// 탈출 / 사망 분기처리
	if (!bIsSurvived)
	{
		if (KillerText)
		{
			const FName Killer = GI->GetKillerMonster();
			KillerText->SetText(FText::Format(
				FTextFormat::FromString("{0}에게 사망"),
				FText::FromName(Killer)));
		}
	}

	// 공통 부분
	float SurvivalTime = GI->GetSurvivalTime();
	SurvivalTimeText->SetText(FText::AsNumber(SurvivalTime));

	if (KillCountList && KillCountEntryClass)
	{
		KillCountList->ClearChildren();

		TMap<FName, int32> KilledMonsters;
		GI->GetKilledMonsters(KilledMonsters);

		KilledMonsterText->SetText(FText::AsNumber(KilledMonsters.Num()));

		for (const TPair<FName, int32>& Pair : KilledMonsters)
		{
			UKillCountEntryWidget* Entry = CreateWidget<UKillCountEntryWidget>(this, KillCountEntryClass);
			if (!Entry)
				return;

			Entry->SetKillCountEntry(FText::FromName(Pair.Key), Pair.Value);
			KillCountList->AddChildToVerticalBox(Entry);
		}
	}

	if (BackpackSlotPanel)
	{
		BackpackSlotPanel->SetInventory(OwnerCharacter->GetPlayerInventoryComponent(), nullptr);
		BackpackSlotPanel->SetContainerName(FText::FromString(TEXT("가방")));
	}
}

void UResultScreenWidget::OnOKBtnClicked()
{
	if (UUIManager* UIManager = UUIManager::Get(this))
	{
		UIManager->PopScreen();
	}
}
