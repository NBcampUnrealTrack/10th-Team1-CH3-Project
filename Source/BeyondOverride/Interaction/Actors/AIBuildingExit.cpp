// Fill out your copyright notice in the Description page of Project Settings.

#include "Interaction/Actors/AIBuildingExit.h"

#include "GameFlow/BOGameInstance.h"
#include "GameFlow/BOGameMode.h"

AAIBuildingExit::AAIBuildingExit()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (MeshFinder.Succeeded())
		MeshComp->SetStaticMesh(MeshFinder.Object);

	PromptData.Title = FText::FromString(TEXT("AI 건물 출구"));
	PromptData.ActionText = FText::FromString(TEXT("[E] 키를 눌러 퇴장하세요"));
	PromptData.HoldSeconds = 3.f;
	PromptData.bMoveCancel = true;
	PromptData.DisableReason = FText::FromString(TEXT("탈출이 불가합니다."));
}

void AAIBuildingExit::BeginPlay()
{
	Super::BeginPlay();

	if (!IsDefenseStarted())
	{
		PromptData.bEnabled = true;
	}
}

bool AAIBuildingExit::CanInteract(AActor* Interactor, FText& OutReason) const
{
	OutReason = PromptData.DisableReason;

	return !IsDefenseStarted();
}

void AAIBuildingExit::PerformInteract(AActor* Interactor)
{
	if (GetWorld())
	{
		if (ABOGameMode* GameMode = GetWorld()->GetAuthGameMode<ABOGameMode>())
		{
			GameMode->ExitAIBuilding();
		}
	}
}

bool AAIBuildingExit::IsDefenseStarted() const
{
	UBOGameInstance* GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>();
	if (!GameInstance)
	{
		return false;
	}

	return GameInstance->GetIsDefenseStarted();
}
