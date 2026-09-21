#include "Interaction/Actors/KeycardDoorController.h"

#include "DataAssets/BODataAsset.h"
#include "GameFlow/BOGameInstance.h"
#include "GameFlow/BOGameMode.h"
#include "Player/ActorComponent/PlayerInventoryComponent.h"
#include "Player/Character/BOCharacter.h"
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

	if (HasPlayerKeyCard())
	{
		PromptData.bEnabled = true;
	}
}

bool AKeycardDoorController::CanInteract(AActor* Interactor, FText& OutReason) const
{
	OutReason = PromptData.DisableReason;

	// 키카드 보유하고있는지 로직추가
	return HasPlayerKeyCard();
}

void AKeycardDoorController::PerformInteract(AActor* Interactor)
{
	if (GetWorld())
	{
		if (ABOGameMode* GameMode = GetWorld()->GetAuthGameMode<ABOGameMode>())
		{
			GameMode->EnterServerRoom();
		}
	}
}

bool AKeycardDoorController::HasPlayerKeyCard() const
{
	if (!GetWorld() || !GetWorld()->GetFirstPlayerController())
	{
		return false;
	}

	UBOGameInstance* GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>();
	if (!GameInstance)
	{
		return false;
	}

	UBODataAsset* DataAsset = GameInstance->GetBODataAsset();
	if (!DataAsset)
	{
		return false;
	}

	FName KeyCardID = DataAsset->GetKeyCardID();

	if (ABOCharacter* Character = GetWorld()->GetFirstPlayerController()->GetPawn<ABOCharacter>())
	{
		if (UPlayerInventoryComponent* InventoryComponent = Character->GetPlayerInventoryComponent())
		{
			if (InventoryComponent->FindItemIndex(KeyCardID) != INDEX_NONE)
			{
				return true;
			}
		}
	}

	return false;
}
