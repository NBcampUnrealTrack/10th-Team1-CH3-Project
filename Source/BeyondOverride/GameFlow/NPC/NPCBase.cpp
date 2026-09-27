// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/NPC/NPCBase.h"

#include "NPCAIController.h"

#include "ActorComponent/BehaviorComponent.h"
#include "ActorComponent/DialogueComponent.h"
#include "ActorComponent/InteractionComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Interaction/Internal/InteractionChannels.h"

ANPCBase::ANPCBase()
{
	AIControllerClass = ANPCAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	NPCID = FName(TEXT("Default"));

	InteractionComp = CreateDefaultSubobject<UInteractionComponent>(FName(TEXT("Interaction Comp")));
	BehaviorComp = CreateDefaultSubobject<UBehaviorComponent>(FName(TEXT("Behavior Comp")));
	DialogueComp = CreateDefaultSubobject<UDialogueComponent>(FName(TEXT("Dialogue Comp")));

	// set prompt data
}

void ANPCBase::BeginPlay()
{
	Super::BeginPlay();
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
}

FName ANPCBase::GetNPCID() const
{
	return NPCID;
}

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
