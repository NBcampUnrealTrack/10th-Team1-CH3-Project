// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Widgets/NPCInteractionWidget.h"

#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "GameFlow/Manager/DialogueManager.h"
#include "GameFlow/NPC/NPCBase.h"
#include "Interaction/InteractComponent.h"
#include "Logging/BOLog.h"
#include "Player/Character/BOCharacter.h"
#include "UI/Widgets/NPCInteractionButtonWidget.h"

void UNPCInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!GetWorld() || !GetWorld()->GetGameInstance() || !GetWorld()->GetFirstPlayerController())
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

	NPC = Cast<ANPCBase>(Actor);
	if (!NPC)
	{
		return;
	}

	DialogueManager = GetWorld()->GetGameInstance()->GetSubsystem<UDialogueManager>();
	if (!DialogueManager)
	{
		return;
	}

	InteractComponent->SetInteractionEnabled(false);

	InitSetting();
}

void UNPCInteractionWidget::InitSetting()
{
	DialogueInteractionData = {};
	DialogueData = {};

	GetDialogueInteractionData(EDialogueSituation::Idle);
	SetDialogueTextBlock(DialogueInteractionData.Text);

	TArray<ENPCInteractionOption> InteractionOptions{};
	NPC->GetInteractionOptions(InteractionOptions);

	for (ENPCInteractionOption Option : InteractionOptions)
	{
		TObjectPtr<UNPCInteractionButtonWidget> ButtonWidget = CreateButtonWidget(Option);
		AddButton(ButtonWidget);
	}

	if (DialogueBorder)
	{
		DialogueBorder->OnMouseButtonDownEvent.Unbind();
	}
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

TObjectPtr<UNPCInteractionButtonWidget> UNPCInteractionWidget::CreateButtonWidget(ENPCInteractionOption Option)
{
	if (NPCInteractionButtonWidgetClass)
	{
		UNPCInteractionButtonWidget* ButtonWidget = CreateWidget<UNPCInteractionButtonWidget>(GetWorld(), NPCInteractionButtonWidgetClass);

		if (ButtonWidget)
		{
			UE_LOG(LogGameFlow, Warning, TEXT("Create Button"));

			ButtonWidget->OnInteractionButtonClicked.BindUObject(this, &UNPCInteractionWidget::OnInteractionButtonClicked);
			ButtonWidget->SetNPC(NPC);
			ButtonWidget->SetButtonOption(Option);

			FText DisplayName = StaticEnum<ENPCInteractionOption>()->GetDisplayNameTextByValue(static_cast<int64>(Option));
			ButtonWidget->SetButtonText(DisplayName);

			return ButtonWidget;
		}
	}

	return nullptr;
}

TObjectPtr<UNPCInteractionButtonWidget> UNPCInteractionWidget::CreateButtonWidget(EDialogueSituation Situation, FName NextDialogueID, FText Text)
{
	if (NPCInteractionButtonWidgetClass)
	{
		UNPCInteractionButtonWidget* ButtonWidget = CreateWidget<UNPCInteractionButtonWidget>(GetWorld(), NPCInteractionButtonWidgetClass);

		if (ButtonWidget)
		{
			ButtonWidget->OnTalkButtonClicked.BindUObject(this, &UNPCInteractionWidget::OnTalkSituation);
			ButtonWidget->SetNPC(NPC);
			ButtonWidget->SetButtonSituation(Situation);
			ButtonWidget->SetNextDialogueID(NextDialogueID);
			ButtonWidget->SetButtonText(Text);

			return ButtonWidget;
		}
	}

	return nullptr;
}

void UNPCInteractionWidget::GetDialogueInteractionData(EDialogueSituation DialogueSituation)
{
	if (!DialogueManager)
	{
		return;
	}

	FName DialogueID = DialogueManager->GetNPCDialogueInteractionData(NPC->GetNPCID(), DialogueSituation);
	UE_LOG(LogGameFlow, Warning, TEXT("Dialogue Interaction ID : %s"), *DialogueID.ToString());
	DialogueManager->GetDialogueInteractionData(DialogueID, DialogueInteractionData);
}

void UNPCInteractionWidget::SetDialogueTextBlock(FText Dialogue)
{
	if (DialogueTextBlock)
	{
		DialogueTextBlock->SetText(Dialogue);
	}
}

void UNPCInteractionWidget::StartTalk()
{
	if (!DialogueManager)
	{
		return;
	}

	GetDialogueInteractionData(EDialogueSituation::Talk);
	SetDialogueTextBlock(DialogueInteractionData.Text);

	ShowTalkList();
}

void UNPCInteractionWidget::ShowTalkList()
{
	if (!DialogueManager || !ButtonVerticalBox || !NPC)
	{
		return;
	}

	ButtonVerticalBox->ClearChildren();

	TArray<FName> TopicIDs{};
	DialogueManager->GetNPCTopicDatas(NPC->GetNPCID(), TopicIDs);

	for (FName TopicID : TopicIDs)
	{
		FTopicData TopicData{};
		DialogueManager->GetTopicData(TopicID, TopicData);
		FName NextDialogueID = TopicData.NextDialogueID;
		FText Topic = TopicData.Topic;

		TObjectPtr<UNPCInteractionButtonWidget> ButtonWidget = CreateButtonWidget(EDialogueSituation::Talk, NextDialogueID, Topic);
		AddButton(ButtonWidget);
	}
}

void UNPCInteractionWidget::ShowQuestList()
{
}

void UNPCInteractionWidget::HealPlayer()
{
}

void UNPCInteractionWidget::OnDialogueBorderClicked()
{
	if (!DialogueBorder || !ButtonVerticalBox || !DialogueManager)
	{
		return;
	}

	FName NextDialogueID = DialogueData.NextDialogueID;

	if (!ButtonVerticalBox->HasAnyChildren() && NextDialogueID == FName(TEXT("None")))
	{
		InitSetting();

		return;
	}

	OnTalkSituation(NextDialogueID);
}

void UNPCInteractionWidget::OnInteractionButtonClicked(ENPCInteractionOption Option)
{
	switch (Option)
	{
	case ENPCInteractionOption::Dialogue:
		StartTalk();
		break;
	case ENPCInteractionOption::Quest:
		ShowQuestList();
		break;
	case ENPCInteractionOption::Heal:
		HealPlayer();
		break;
	default:
		break;
	}
}

void UNPCInteractionWidget::OnTalkSituation(FName NextDialogueID)
{
	if (!DialogueManager || !ButtonVerticalBox || !NPC)
	{
		return;
	}

	ButtonVerticalBox->ClearChildren();

	/*if (NextDialogueID == FName(TEXT("None")))
	{
		InitSetting();

		return;
	}*/

	DialogueManager->GetDialogueData(NextDialogueID, DialogueData);
	SetDialogueTextBlock(DialogueData.Text);

	TArray<FTalkOption> Options = DialogueData.Options;

	if (Options.Num() == 0)
	{
		if (DialogueBorder)
		{
			DialogueBorder->OnMouseButtonDownEvent.BindUFunction(this, FName(TEXT("OnDialogueBorderClicked")));
		}

		return;
	}

	if (DialogueBorder)
	{
		DialogueBorder->OnMouseButtonDownEvent.Unbind();
	}

	for (FTalkOption Option : Options)
	{
		TObjectPtr<UNPCInteractionButtonWidget> ButtonWidget = CreateButtonWidget(EDialogueSituation::Talk, Option.NextDialogueID, Option.OptionText);
		AddButton(ButtonWidget);
	}
}
