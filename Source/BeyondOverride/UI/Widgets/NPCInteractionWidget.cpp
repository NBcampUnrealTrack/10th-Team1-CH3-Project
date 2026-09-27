// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Widgets/NPCInteractionWidget.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "GameFlow/NPC/NPCBase.h"
#include "Interaction/InteractComponent.h"
#include "Player/Character/BOCharacter.h"
#include "UI/Widgets/NPCInteractionButtonWidget.h"

void UNPCInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!GetWorld() || !GetWorld()->GetFirstPlayerController())
	{
		return;
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

	AActor* Actor = InteractComponent->GetFocusedActor();
	if (!Actor)
	{
		return;
	}

	ANPCBase* NPC = Cast<ANPCBase>(Actor);
	if (!NPC)
	{
		return;
	}

	FName NPCID = NPC->GetNPCID();

	TArray<ENPCInteractionOption> InteractionOptions{};
	NPC->GetInteractionOptions(InteractionOptions);

	for (ENPCInteractionOption Option : InteractionOptions)
	{
		TObjectPtr<UNPCInteractionButtonWidget> ButtonWidget = CreateButtonWidget(NPCID, Option);
		AddButton(ButtonWidget);
	}

	InteractComponent->SetInteractionEnabled(false);
}

void UNPCInteractionWidget::AddButton(UWidget* Button)
{
	if (!Button)
	{
		return;
	}

	if (ButtonVerticalBox)
	{
		ButtonVerticalBox->AddChild(Button);
	}
}

TObjectPtr<UNPCInteractionButtonWidget> UNPCInteractionWidget::CreateButtonWidget(FName NPCID, ENPCInteractionOption Option)
{
	if (NPCInteractionButtonWidgetClass)
	{
		UNPCInteractionButtonWidget* ButtonWidget = CreateWidget<UNPCInteractionButtonWidget>(GetWorld(), NPCInteractionButtonWidgetClass);

		if (ButtonWidget)
		{
			ButtonWidget->SetNPCID(NPCID);
			ButtonWidget->SetButtonOption(Option);

			FText DisplayName = StaticEnum<ENPCInteractionOption>()->GetDisplayNameTextByValue(static_cast<int64>(Option));
			ButtonWidget->SetButtonText(DisplayName);

			return ButtonWidget;
		}
	}

	return nullptr;
}

void UNPCInteractionWidget::SetDialogue(FText Dialogue)
{
	if (DialogueTextBlock)
	{
		DialogueTextBlock->SetText(Dialogue);
	}
}
