#include "UI/Widgets/ResultScreenWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
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
	int32 Hours = FMath::FloorToInt(SurvivalTime / 3600.0f);
	int32 Minutes = FMath::FloorToInt(FMath::Fmod(SurvivalTime, 3600.0f) / 60.0f);
	int32 Seconds = FMath::FloorToInt(FMath::Fmod(SurvivalTime, 60.0f));

	FString TimeString = FString::Printf(TEXT("%d:%02d:%02d"), Hours, Minutes, Seconds);

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
