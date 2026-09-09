#include "Interaction/Actors/ExitActor.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

AExitActor::AExitActor()
{
	// 베이스가 만들어둔 빈 루트에 내 메시를 붙인다.
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	// [맨홀 에셋으로 교체 해야함]
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(
		TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(CubeAsset.Object);
	}

	// 콜리전 채널은 베이스가 PostInitializeComponents 에서 알아서 한다.

	PromptData.Title = FText::FromString(TEXT("탈출구"));
	PromptData.ActionText = FText::FromString(TEXT("[E] 키를 눌러 탈출하세요!"));
	PromptData.HoldSeconds = 0.f;
	PromptData.bMoveCancel = false;
	PromptData.bEnabled = false;

	// 잠겨 있을 때 보여줄 이유 (에디터에서 바꿀 수 있다)
	PromptData.DisableReason =
		FText::FromString(TEXT("아직 탈출할 수 없습니다"));
}

void AExitActor::BeginPlay()
{
	Super::BeginPlay();

	// 에디터에서 설정한 초기 상태를 적용한다.
	bExtractAvailable = bStartAvailable;
}

// 밖에서 부르는 것
void AExitActor::SetExtractAvailable(bool bAvailable)
{
	bExtractAvailable = bAvailable;

	// 프롬프트를 따로 갱신할 필요가 없다.
	// 탐지기가 매 프레임 GetInteractPrompt() 를 다시 물어보고,
	// 값이 바뀌면 알아서 UI 에 통보한다.
}

// 상호작용
bool AExitActor::CanInteract(AActor *Interactor, FText &OutReason) const
{
	// 베이스가 OutReason 에 PromptData.DisableReason 을 담아준다.
	// 에디터에서 이 액터를 꺼둔 경우 여기서 바로 끝난다.
	if (!Super::CanInteract(Interactor, OutReason))
	{
		return false;
	}

	// 아직 열리지 않았으면 베이스가 담아준 이유를 그대로 쓴다.
	return bExtractAvailable;
}

void AExitActor::PerformInteract(AActor *Interactor)
{
	// 신호만 쏜다. 이 액터는 게임모드도 결과 화면도 모른다.
	// 게임 흐름 담당이 이걸 듣고 레이드를 종료시킨다:
	//   Exit->OnExtractRequested.AddDynamic(this, &A...::HandleExtract);
	OnExtractRequested.Broadcast(this, Interactor);
}
