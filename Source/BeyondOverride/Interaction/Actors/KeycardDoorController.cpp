#include "Interaction/Actors/KeycardDoorController.h"

#include "GameFlow/BOGameMode.h"
#include "UObject/ConstructorHelpers.h"

AKeycardDoorController::AKeycardDoorController()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (MeshFinder.Succeeded())
		MeshComp->SetStaticMesh(MeshFinder.Object);

	PromptData.Title = FText::FromString(TEXT("문 개방 스위치"));
	PromptData.ActionText = FText::FromString(TEXT("[E] 키를 눌러 문을 개방하세요"));
	PromptData.HoldSeconds = 3.f;
	PromptData.bMoveCancel = true;
	PromptData.DisableReason = FText::FromString(TEXT("카드키를 보유하고 있지 않습니다."));
}

void AKeycardDoorController::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		if (ABOGameMode* GameMode = GetWorld()->GetAuthGameMode<ABOGameMode>())
		{
			if (GameMode->IsKeyCardAcquired())
			{
				PromptData.bEnabled = true;
			}
		}
	}
}

bool AKeycardDoorController::CanInteract(AActor* Interactor, FText& OutReason) const
{
	OutReason = PromptData.DisableReason;

	// 키카드 보유하고있는지 로직추가
	if (GetWorld())
	{
		if (ABOGameMode* GameMode = GetWorld()->GetAuthGameMode<ABOGameMode>())
		{
			return GameMode->IsKeyCardAcquired();
		}
	}

	return false;
}

void AKeycardDoorController::PerformInteract(AActor* Interactor)
{
	if (GetWorld())
	{
		if (ABOGameMode* GameMode = GetWorld()->GetAuthGameMode<ABOGameMode>())
		{
			GameMode->ToEnding();
		}
	}
}
