// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Widgets/NPCInteractionButtonWidget.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameFlow/Manager/NPCManager.h"
#include "GameFlow/NPC/NPCAIController.h"
#include "GameFlow/NPC/NPCBase.h"
#include "Interaction/InteractComponent.h"
#include "Logging/BOLog.h"
#include "Player/Character/BOCharacter.h"
#include "UI/Manager/UIManager.h"
#include "UI/Widgets/ShopScreenWidget.h"

void UNPCInteractionButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	/*NPC = nullptr;
	Option = ENPCInteractionOption::None;
	Situation = EDialogueSituation::None;*/

	ShopItems.Empty();

	if (InteractionButton)
	{
		InteractionButton->OnClicked.AddDynamic(this, &UNPCInteractionButtonWidget::OnButtonClicked);
	}
}

void UNPCInteractionButtonWidget::SetNPC(TObjectPtr<ANPCBase> InNPC)
{
	NPC = InNPC;
}

void UNPCInteractionButtonWidget::SetButtonOption(ENPCInteractionOption InOption)
{
	UE_LOG(LogGameFlow, Warning, TEXT("SetButtonOption"));

	Option = InOption;
}

void UNPCInteractionButtonWidget::SetButtonSituation(EDialogueSituation InSituation)
{
	Situation = InSituation;
}

void UNPCInteractionButtonWidget::SetNextDialogueID(FName ID)
{
	NextDialogueID = ID;
}

void UNPCInteractionButtonWidget::SetButtonText(FText Text)
{
	if (ButtonTextBlock)
	{
		ButtonTextBlock->SetText(Text);
	}
}

void UNPCInteractionButtonWidget::OnButtonClicked()
{
	UE_LOG(LogGameFlow, Warning, TEXT("OnButtonClicked Called"));
	if (Option != ENPCInteractionOption::None)
	{
		UE_LOG(LogGameFlow, Warning, TEXT("Option Exist"));
		OnInteractionClicked();
	}
	else if (Situation == EDialogueSituation::Talk)
	{
		UE_LOG(LogGameFlow, Warning, TEXT("Situation Exist"));
		OnTalkSituation();
	}
	else if (Option == ENPCInteractionOption::None)
	{
		UE_LOG(LogGameFlow, Warning, TEXT("No Option"));
	}
	else if (Situation == EDialogueSituation::None)
	{
		UE_LOG(LogGameFlow, Warning, TEXT("No Situation"));
	}
}

void UNPCInteractionButtonWidget::OnInteractionClicked()
{
	switch (Option)
	{
	case ENPCInteractionOption::Dialogue:
		// show topic list
		Talk();
		break;
	case ENPCInteractionOption::Shop:
		OpenShop();
		break;
	case ENPCInteractionOption::Quest:
		// show quest list
		break;
	case ENPCInteractionOption::Heal:
		// change dialogue and heal player's stat. heal is free!
		break;
	case ENPCInteractionOption::Exit:
		UE_LOG(LogGameFlow, Warning, TEXT("Exit"));
		Goodbye();
		break;
	default:
		break;
	}
}

void UNPCInteractionButtonWidget::OnTalkSituation()
{
	OnTalkButtonClicked.ExecuteIfBound(NextDialogueID);
}

void UNPCInteractionButtonWidget::Talk()
{
	OnInteractionButtonClicked.ExecuteIfBound(Option);
}

void UNPCInteractionButtonWidget::OpenShop()
{
	if (!GetWorld() || !GetWorld()->GetGameInstance())
	{
		return;
	}

	UNPCManager* NPCManager = GetWorld()->GetGameInstance()->GetSubsystem<UNPCManager>();
	if (!NPCManager)
	{
		return;
	}

	UUIManager* UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();
	if (!UIManager)
	{
		return;
	}

	NPCManager->GetNPCShopItems(NPC->GetNPCID(), ShopItems);

	UUserWidget* Widget = UIManager->PushScreen(EUIScreen::ShopScreen, EUIInputMode::UIOnly);
	if (UShopScreenWidget* ShopScreen = Cast<UShopScreenWidget>(Widget))
	{
		ShopScreen->SetShopItems(ShopItems);
	}
}

void UNPCInteractionButtonWidget::Goodbye()
{
	UE_LOG(LogGameFlow, Warning, TEXT("Goodbye Called"));
	if (!GetWorld() || !GetWorld()->GetGameInstance() || !GetWorld()->GetFirstPlayerController())
	{
		return;
	}

	if (UUIManager* UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>())
	{
		UIManager->PopScreen();
		UIManager->ShowScreen(EUIScreen::HUD, EUIInputMode::GameOnly);
	}

	ABOCharacter* Character = GetWorld()->GetFirstPlayerController()->GetPawn<ABOCharacter>();
	if (!Character)
	{
		return;
	}

	UInteractComponent* InteractComponent = Character->GetInteractComponent();
	if (!InteractComponent)
	{
		return;
	}

	InteractComponent->SetInteractionEnabled(true);

	if (!NPC)
	{
		return;
	}

	AController* Controller = NPC->GetController();
	if (!Controller)
	{
		return;
	}

	AAIController* AIController = Cast<AAIController>(Controller);
	if (!AIController)
	{
		return;
	}

	UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
	if (!BlackboardComponent)
	{
		return;
	}

	BlackboardComponent->SetValueAsBool(TEXT("IsInteracting"), false);
	BlackboardComponent->SetValueAsBool(TEXT("IsLookingAtPlayer"), false);
}
