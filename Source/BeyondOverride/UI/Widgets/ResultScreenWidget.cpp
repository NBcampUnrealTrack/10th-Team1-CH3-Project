#include "UI/Widgets/ResultScreenWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "DataTables/Monster/MonsterInfo.h"
#include "GameFlow/BOGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ActorComponent/PlayerInventoryComponent.h"
#include "Player/Character/BOCharacter.h"
#include "UI/Manager/UIManager.h"
#include "UI/Widgets/ItemSlotPanelWidget.h"
#include "UI/Widgets/KillCountEntryWidget.h"
#include "UObject/ConstructorHelpers.h"

UResultScreenWidget::UResultScreenWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UKillCountEntryWidget> KillCountEntryWBP(TEXT("/Game/UI/Widgets/WBP_KillCountEntry"));
	if (KillCountEntryWBP.Succeeded())
	{
		KillCountEntryClass = KillCountEntryWBP.Class;
	}
}

void UResultScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (OKBtn)
	{
		OKBtn->OnClicked.AddDynamic(this, &UResultScreenWidget::OnOKBtnClicked);
	}

	UBOGameInstance* GI = Cast<UBOGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (!GI)
		return;

	ABOCharacter* OwnerCharacter = Cast<ABOCharacter>(GetOwningPlayerPawn());
	if (!OwnerCharacter)
		return;

	bIsSurvived = (GI->GetFarmingResult() == EStageResult::Success);

	// 탈출 / 사망 분기처리
	if (!bIsSurvived)
	{
		const FName Killer = GI->GetKillerMonster();

		if (Killer != "None")
		{
			if (KillerText)
				KillerText->SetText(FText::FromString(Killer.ToString() + TEXT("에게")));

			if (MonsterImage && MonsterDataTable)
			{
				MonsterImage->SetVisibility(ESlateVisibility::Visible);

				TArray<FMonsterInfo*> AllRows;
				if (FMonsterInfo* FoundRow = MonsterDataTable->FindRow<FMonsterInfo>(Killer, TEXT("Killer_FindByName")))
				{
					MonsterImage->SetBrushFromTexture(FoundRow->MonsterImage);
				}
			}
			if (TXT_Dead)
				TXT_Dead->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			if (MonsterImage)
				MonsterImage->SetVisibility(ESlateVisibility::Collapsed);
			if (TXT_Dead)
				TXT_Dead->SetVisibility(ESlateVisibility::Collapsed);
			if (KillerText)
				KillerText->SetText(FText::FromString(TEXT("자살하셨군요.. 왜 그러셨습니까?")));
		}
	}

	// 공통 부분
	float SurvivalTime = GI->GetSurvivalTime();
	int32 Hours = FMath::FloorToInt(SurvivalTime / 3600.0f);
	int32 Minutes = FMath::FloorToInt(FMath::Fmod(SurvivalTime, 3600.0f) / 60.0f);
	int32 Seconds = FMath::FloorToInt(FMath::Fmod(SurvivalTime, 60.0f));

	FString TimeString;

	if (Hours)
	{
		TimeString = FString::Printf(TEXT("%d:%02d:%02d"), Hours, Minutes, Seconds);
	}
	else
	{
		TimeString = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
	}

	SurvivalTimeText->SetText(FText::FromString(TimeString));

	UE_LOG(LogTemp, Warning, TEXT("KillCountList is %s"),
		   KillCountList ? TEXT("VALID") : TEXT("NULL"));

	UE_LOG(LogTemp, Warning, TEXT("KillCountEntryClass is %s"),
		   KillCountEntryClass ? TEXT("VALID") : TEXT("NULL"));

	if (KillCountList && KillCountEntryClass)
	{
		KillCountList->ClearChildren();

		TMap<FName, int32> KilledMonsters;
		GI->GetKilledMonsters(KilledMonsters);

		int32 TotalKilled = 0;
		for (auto& Elem : KilledMonsters)
			TotalKilled += Elem.Value;

		KilledMonsterText->SetText(FText::AsNumber(TotalKilled));

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

void UResultScreenWidget::NativeDestruct()
{
	if (OKBtn)
	{
		OKBtn->OnClicked.RemoveDynamic(this, &UResultScreenWidget::OnOKBtnClicked);
	}
}

void UResultScreenWidget::OnOKBtnClicked()
{
	if (UUIManager* UIManager = UUIManager::Get(this))
	{
		UIManager->PopScreen();
	}
}
