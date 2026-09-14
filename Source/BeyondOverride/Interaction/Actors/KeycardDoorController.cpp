#include "Interaction/Actors/KeycardDoorController.h"

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
	PromptData.HoldSeconds = 1.f;
	PromptData.bMoveCancel = true;
	PromptData.DisableReason = FText::FromString(TEXT("카드키를 보유하고 있지 않습니다."));
}

bool AKeycardDoorController::CanInteract(AActor* Interactor, FText& OutReason) const
{
	OutReason = PromptData.DisableReason;

	// 키카드 보유하고있는지 로직추가

	return true;
}

