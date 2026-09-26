// Fill out your copyright notice in the Description page of Project Settings.

#include "Interaction/Actors/ServerComputerActor.h"

#include "DataAssets/BODataAsset.h"
#include "GameFlow/BOGameInstance.h"
#include "GameFlow/BOGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/BOLog.h"
#include "UI/Manager/UIManager.h"
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

	PromptData.Title = FText::FromString(TEXT("서버 메인 컴퓨터"));
	PromptData.ActionText = FText::FromString(TEXT("[E] 키를 눌러 서버를 해킹하세요."));
	PromptData.HoldSeconds = 5.f;
	PromptData.bMoveCancel = true;
	PromptData.bEnabled = true;

	HackingTime = 10.0f;
	IsHacked = false;
}

void AServerComputerActor::PerformInteract(AActor* Interactor)
{
	ABOGameMode* GM = Cast<ABOGameMode>(UGameplayStatics::GetGameMode(this));
	if (!GM)
		return;

	if (!IsHacked)
	{
		GM->StartDefense();

		OnHackingStarted();
	}
	else
	{
		GM->ClearGame();
	}
}

void AServerComputerActor::OnHackingStarted()
{
	if (!GetWorld())
	{
		return;
	}

	UBOGameInstance* GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>();
	if (!GameInstance)
	{
		return;
	}

	UBODataAsset* DataAsset = GameInstance->GetBODataAsset();
	if (!DataAsset)
	{
		return;
	}

	HackingTime = DataAsset->GetTotalDefenseTime();

	PromptData.bEnabled = false;
	PromptData.DisableReason = FText::FromString(TEXT("해킹 중..."));

	GetWorld()->GetTimerManager().SetTimer(
		HackingTimer,
		this,
		&AServerComputerActor::OnHackingCompleted,
		HackingTime,
		false);

	UUIManager::Get(this)->ShowNotification(
		FText::FromString(TEXT("서버 해킹 시작")),
		FText::FromString(TEXT("해킹이 완료될 때 까지 살아남으세요.")),
		5.0f);

	PlayInteractionSound(1, 2);
}

void AServerComputerActor::OnHackingCompleted()
{
	UE_LOG(LogGameFlow, Warning, TEXT("OnHackingCompleted Called"));
	IsHacked = true;

	PromptData.bEnabled = true;
	PromptData.ActionText = FText::FromString(TEXT("[E] 키를 눌러 폭파하세요!"));

	UUIManager::Get(this)->ShowNotification(
		FText::FromString(TEXT("서버 해킹 완료")),
		FText::FromString(TEXT("AI 기업 건물 폭파가 가능합니다.")),
		5.0f);
}
