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
	PromptData.bEnabled = false;
	PromptData.DisableReason = FText::FromString(TEXT("폐쇄된 탈출 구역 입니다. 다른 탈출 구역을 이용하세요."));
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

void AExitControllerActor::SetControllerAvailable(bool bNewEnabled, const FText& Reason)
{
	PromptData.bEnabled = bNewEnabled;
	if (!bNewEnabled && !Reason.IsEmpty())
	{
		PromptData.DisableReason = Reason;
	}
}

void AExitControllerActor::PerformInteract(AActor* Interactor)
{
	if (TargetExit)
	{
		PromptData.bEnabled = false;
		PromptData.DisableReason = FText::FromString(TEXT("탈출 개방 장치 작동 중..."));

		GetWorld()->GetTimerManager().SetTimer(
			ControlTimer,
			this,
			&AExitControllerActor::SetExitActorOpenTimer,
			ControlTime,
			false);

		if (OnExtractControlRequested.IsBound())
		{
			OnExtractControlRequested.Broadcast(this, Interactor);
		}
	}
}

void AExitControllerActor::SetExitActorOpenTimer()
{
	TargetExit->SetExtractAvailable(true);
	PromptData.DisableReason = FText::FromString(TEXT("탈출 개방 장치 작동 완료"));
}
