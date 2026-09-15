#include "Interaction/Actors/ScavengeActor.h"

#include "GameFlow/BOGameInstance.h"
#include "GameFlow/BOGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AScavengeActor::AScavengeActor()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(TEXT("/Game/Assets/Items/Decorations/Locker/SM_ClosedLocker.SM_ClosedLocker"));
	if (MeshFinder.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshFinder.Object);
	}

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
