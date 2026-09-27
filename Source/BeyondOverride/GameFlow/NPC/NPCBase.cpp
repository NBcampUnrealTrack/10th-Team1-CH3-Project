// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/NPC/NPCBase.h"

#include "NPCAIController.h"

// #include "ActorComponent/BehaviorComponent.h"
// #include "ActorComponent/DialogueComponent.h"
// #include "ActorComponent/InteractionComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFlow/Manager/NPCManager.h"
#include "Interaction/InteractComponent.h"
#include "Interaction/Internal/InteractionChannels.h"
#include "Player/Character/BOCharacter.h"
#include "UI/Manager/UIManager.h"

ANPCBase::ANPCBase()
{
	AIControllerClass = ANPCAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	/*InteractionComp = CreateDefaultSubobject<UInteractionComponent>(FName(TEXT("Interaction Comp")));
	BehaviorComp = CreateDefaultSubobject<UBehaviorComponent>(FName(TEXT("Behavior Comp")));
	DialogueComp = CreateDefaultSubobject<UDialogueComponent>(FName(TEXT("Dialogue Comp")));*/
}

void ANPCBase::BeginPlay()
{
	Super::BeginPlay();

	if (!GetWorld() || !GetWorld()->GetGameInstance())
	{
		return;
	}

	UNPCManager* NPCManager = GetWorld()->GetGameInstance()->GetSubsystem<UNPCManager>();
	if (!NPCManager)
	{
		return;
	}

	NPCManager->GetNPCData(NPCID, NPCData);

	// set prompt data
	PromptData.Title = FText::FromString(NPCData.NPCName.ToString());
	PromptData.ActionText = FText::FromString(TEXT("[E] 키를 눌러 대화하기"));
}

void ANPCBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TArray<UPrimitiveComponent*> Prims;
	GetComponents<UPrimitiveComponent>(Prims);

	for (UPrimitiveComponent* P : Prims)
	{
		if (P && P->IsCollisionEnabled())
		{
			P->SetCollisionResponseToChannel(ECC_Interaction, ECR_Block);
			P->SetGenerateOverlapEvents(true);
		}
	}
}

FInteractPrompt ANPCBase::GetInteractPrompt(AActor* Interactor) const
{
	return PromptData;
}

void ANPCBase::OnInteractComplete(AActor* Interactor)
{
	PerformInteract(Interactor);
}

void ANPCBase::PerformInteract(AActor* Interactor)
{
	// show interaction widget
	if (!GetWorld() || !GetWorld()->GetGameInstance())
	{
		return;
	}

	UUIManager* UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();
	if (!UIManager)
	{
		return;
	}

	UIManager->PopScreen();
	UIManager->ShowScreen(EUIScreen::NPCInteraction, EUIInputMode::UIOnly);
}

FName ANPCBase::GetNPCID() const
{
	return NPCID;
}

/*
TObjectPtr<UInteractionComponent> ANPCBase::GetInteractionComp() const
{
	return InteractionComp;
}

TObjectPtr<UBehaviorComponent> ANPCBase::GetBehaviorComp() const
{
	return BehaviorComp;
}

TObjectPtr<UDialogueComponent> ANPCBase::GetDialogueComp() const
{
	return DialogueComp;
}
*/

void ANPCBase::GetInteractionOptions(TArray<ENPCInteractionOption>& Options) const
{
	Options = NPCData.InteractionOptions;
}
