// Fill out your copyright notice in the Description page of Project Settings.

#include "Interaction/Actors/ServerComputerActor.h"

#include "GameFlow/BOGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AServerComputerActor::AServerComputerActor()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(TEXT("/Game/Assets/Maps/SciFiWorld/Modules/Meshes/SM_MWall07-350x350-1.SM_MWall07-350x350-1"));
	if (MeshFinder.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshFinder.Object);
	}

	PromptData.Title = FText::FromString(TEXT("폭파하기"));
	PromptData.ActionText = FText::FromString(TEXT("[E] 키를 눌러 JOY를 표하세요!"));
	PromptData.HoldSeconds = 5.f;
	PromptData.bMoveCancel = true;
	PromptData.bEnabled = true;
}

void AServerComputerActor::PerformInteract(AActor* Interactor)
{
	ABOGameMode* GM = Cast<ABOGameMode>(UGameplayStatics::GetGameMode(this));
	if (!GM)
		return;
	GM->Explosion();
}
