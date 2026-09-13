#include "Interaction/Actors/ExitActor.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFlow/BOGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AExitActor::AExitActor()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	// [맨홀 에셋으로 교체 해야함]
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(
		TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(CubeAsset.Object);
	}

	PromptData.Title = FText::FromString(TEXT("탈출구"));
	PromptData.ActionText = FText::FromString(TEXT("[E] 키를 눌러 탈출하세요!"));
	PromptData.HoldSeconds = 0.f;
	PromptData.bMoveCancel = false;
	PromptData.bEnabled = false;
	PromptData.DisableReason =
		FText::FromString(TEXT("아직 탈출할 수 없습니다"));
}

// 밖에서 부르는 것
void AExitActor::SetExtractAvailable(bool bAvailable)
{
	PromptData.bEnabled = bAvailable;

	// 프롬프트를 따로 갱신할 필요가 없다.
	// 탐지기가 매 프레임 GetInteractPrompt() 를 다시 물어보고,
	// 값이 바뀌면 알아서 UI 에 통보한다.
}

void AExitActor::PerformInteract(AActor* Interactor)
{
	UBOGameInstance* GI = Cast<UBOGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (!GI)
		return;
	GI->EndFarming(EFarmingResult::Success);
}
