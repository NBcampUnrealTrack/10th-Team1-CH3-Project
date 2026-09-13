#include "Interaction/Actors/ExitControllerActor.h"

#include "Interaction/Actors/ExitActor.h"
#include "UObject/ConstructorHelpers.h"

AExitControllerActor::AExitControllerActor()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(TEXT("/Game/Assets/Items/Decorations/Radio/SM_Radio.SM_Radio"));
	if (MeshFinder.Succeeded())
		MeshComp->SetStaticMesh(MeshFinder.Object);

	PromptData.Title = FText::FromString(TEXT("탈출 개방 장치"));
	PromptData.ActionText = FText::FromString(TEXT("[E] 키를 눌러 개방시키세요."));
	PromptData.HoldSeconds = 0.f;
	PromptData.bMoveCancel = false;
	PromptData.bEnabled = true;
}

void AExitControllerActor::BeginPlay()
{
	Super::BeginPlay();

	if (!TargetExit)
	{
		PromptData.bEnabled = false;
		PromptData.DisableReason = FText::FromString(TEXT("연결된 탈출구가 없습니다."));
	}
}

void AExitControllerActor::PerformInteract(AActor* Interactor)
{
	if (TargetExit)
	{
		PromptData.bEnabled = false;
		PromptData.DisableReason = FText::FromString(TEXT("탈출 개방 장치가 작동하였습니다."));
		TargetExit->SetExtractAvailable(true);
	}
}
