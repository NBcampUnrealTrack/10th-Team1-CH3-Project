#include "Interaction/Actors/ScavengeActor.h"

#include "GameFlow/BOGameInstance.h"

AScavengeActor::AScavengeActor()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	PromptData.Title = FText::FromString(TEXT("밖으로 나가기"));
	PromptData.ActionText = FText::FromString(TEXT("[E] 키를 눌러 밖으로 나가세요"));
	PromptData.HoldSeconds = 2.f;
	PromptData.bMoveCancel = true;
	PromptData.bEnabled = true;
}

void AScavengeActor::PerformInteract(AActor* Interactor)
{
	UBOGameInstance* GI = GetWorld()->GetGameInstance<UBOGameInstance>();
	if (!GI)
		return;
	GI->StartFarming();
}
