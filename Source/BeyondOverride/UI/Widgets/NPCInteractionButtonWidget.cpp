// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Widgets/NPCInteractionButtonWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameFlow/Manager/NPCManager.h"
#include "Interaction/InteractComponent.h"
#include "Logging/BOLog.h"
#include "Player/Character/BOCharacter.h"
#include "UI/Manager/UIManager.h"
#include "UI/Widgets/ShopScreenWidget.h"

void UNPCInteractionButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ShopItems.Empty();

	if (InteractionButton)
	{
		InteractionButton->OnClicked.AddDynamic(this, &UNPCInteractionButtonWidget::OnInteractionButtonClicked);
	}
}

void UNPCInteractionButtonWidget::SetNPCID(FName ID)
{
	NPCID = ID;
}

void UNPCInteractionButtonWidget::SetButtonOption(ENPCInteractionOption InOption)
{
	Option = InOption;
}

void UNPCInteractionButtonWidget::SetButtonText(FText Text)
{
	if (ButtonTextBlock)
	{
		ButtonTextBlock->SetText(Text);
	}
}

void UNPCInteractionButtonWidget::OnInteractionButtonClicked()
{
	switch (Option)
	{
	case ENPCInteractionOption::Dialogue:
		// show dialogue list
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
		Goodbye();
		break;
	default:
		break;
	}
}

void UNPCInteractionButtonWidget::OpenShop()
{
	if (!GetWorld() || !GetWorld()->GetGameInstance() || !GetWorld()->GetFirstPlayerController())
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

	NPCManager->GetNPCShopItems(NPCID, ShopItems);

	UUserWidget* Widget = UIManager->PushScreen(EUIScreen::ShopScreen, EUIInputMode::UIOnly);
	if (UShopScreenWidget* ShopScreen = Cast<UShopScreenWidget>(Widget))
	{
		ShopScreen->SetShopItems(ShopItems);
	}
}

void UNPCInteractionButtonWidget::Goodbye()
{
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
}
