#include "UI/Widgets/ResultScreenWidget.h"

#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "GameFlow/BOGameInstance.h"
#include "UI/Manager/UIManager.h"

void UResultScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (OKBtn)
	{
		OKBtn->OnClicked.AddDynamic(this, &UResultScreenWidget::OnOKBtnClicked);
	}

	SetResult();
	SetSurvivalTime();
	SetKillerMonster();
	SetKilledMonster();
}

void UResultScreenWidget::OnOKBtnClicked()
{
	if (UUIManager* UIManager = UUIManager::Get(this))
	{
		UIManager->PopScreen();
	}
}

void UResultScreenWidget::SetResult()
{
	if (!GetWorld() || !Result)
	{
		return;
	}

	UBOGameInstance* GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>();
	if (!GameInstance)
	{
		return;
	}

	EFarmingResult FarmingResult = GameInstance->GetFarmingResult();

	if (FarmingResult == EFarmingResult::Success)
	{
		Result->SetText(FText::FromString(TEXT("무사히 탈출하였습니다.")));
		SetDeadResult(ESlateVisibility::Hidden);
	}
	else if (FarmingResult == EFarmingResult::Fail)
	{
		Result->SetText(FText::FromString(TEXT("사망했습니다.")));
		SetDeadResult(ESlateVisibility::Visible);
	}
}

void UResultScreenWidget::SetDeadResult(ESlateVisibility InVisibility)
{
	if (DeadResult)
	{
		DeadResult->SetVisibility(InVisibility);
	}
}

void UResultScreenWidget::SetSurvivalTime()
{
	if (!GetWorld() || !SurvivalTime)
	{
		return;
	}

	UBOGameInstance* GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>();
	if (!GameInstance)
	{
		return;
	}

	int32 Time = FMath::FloorToInt(GameInstance->GetSurvivalTime());
	int32 Minute = Time / 60;
	int32 Second = Time % 60;

	SurvivalTime->SetText(FText::FromString(FString::Format(TEXT("{0}:{1}"), {Minute, Second})));
}

void UResultScreenWidget::SetKillerMonster()
{
	if (!GetWorld() || !SurvivalTime)
	{
		return;
	}

	UBOGameInstance* GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>();
	if (!GameInstance)
	{
		return;
	}

	KillerMonster->SetText(FText::FromString(FString::Format(TEXT("적{0}에게"), {GameInstance->GetKillerMonster().ToString()})));
}

void UResultScreenWidget::SetKilledMonster()
{
	if (!GetWorld() || !KilledMonster)
	{
		return;
	}

	UBOGameInstance* GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>();
	if (!GameInstance)
	{
		return;
	}

	TMap<FName, int32> Monsters{};
	GameInstance->GetTotalKilledMonsters(Monsters);
	int32 Count = Monsters.Num();

	KilledMonster->SetText(FText::FromString(FString::FromInt(Count)));
}
