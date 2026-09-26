// Fill out your copyright notice in the Description page of Project Settings.

#include "Interaction/Actors/AIBuildingEntrance.h"

#include "GameFlow/BOGameInstance.h"
#include "GameFlow/BOGameMode.h"

AAIBuildingEntrance::AAIBuildingEntrance()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (MeshFinder.Succeeded())
		MeshComp->SetStaticMesh(MeshFinder.Object);

	PromptData.Title = FText::FromString(TEXT("AI 건물 입구"));
	PromptData.ActionText = FText::FromString(TEXT("[E] 키를 눌러 입장하세요"));
	PromptData.HoldSeconds = 3.f;
	PromptData.bMoveCancel = true;
	PromptData.DisableReason = FText::FromString(TEXT("보스 처치 후 입장 가능합니다."));
}

void AAIBuildingEntrance::BeginPlay()
{
	Super::BeginPlay();

	if (IsBossDefeated())
	{
		PromptData.bEnabled = true;
	}
}

bool AAIBuildingEntrance::CanInteract(AActor* Interactor, FText& OutReason) const
{
	OutReason = PromptData.DisableReason;

	return IsBossDefeated();
}

void AAIBuildingEntrance::PerformInteract(AActor* Interactor)
{
	if (GetWorld())
	{
		if (ABOGameMode* GameMode = GetWorld()->GetAuthGameMode<ABOGameMode>())
		{
			GameMode->EnterAIBuilding();
		}
	}
}

bool AAIBuildingEntrance::IsBossDefeated() const
{
	UBOGameInstance* GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>();
	if (!GameInstance)
	{
		return false;
	}

	return GameInstance->GetIsBossDefeated();
}
