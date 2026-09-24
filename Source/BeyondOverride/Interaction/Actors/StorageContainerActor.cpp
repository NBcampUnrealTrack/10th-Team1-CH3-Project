#include "Interaction/Actors/StorageContainerActor.h"

#include "GameFlow/BOGameInstance.h"
#include "Player/ActorComponent/InventoryComponent.h"
#include "UI/Manager/UIManager.h"
#include "UI/Widgets/InventoryScreenWidget.h"

AStorageContainerActor::AStorageContainerActor()
{
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMeshComp);

	StaticMeshComp->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel14);                                         // Container
	StaticMeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel13, ECollisionResponse::ECR_Overlap); // SpawnVolume

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	PromptData.Title = FText::FromString(TEXT("상자"));
	PromptData.ActionText = FText::FromString(TEXT("[E] 키를 눌러 아이템을 획득하세요"));
	PromptData.HoldSeconds = 3.f;
	PromptData.bMoveCancel = true;
	PromptData.bEnabled = true;
}

void AStorageContainerActor::BeginPlay()
{
	Super::BeginPlay();

	StaticMeshComp->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel14);                                         // Container
	StaticMeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel13, ECollisionResponse::ECR_Overlap); // SpawnVolume

	if (GetWorld())
	{
		if (UBOGameInstance* GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>())
		{
			if (GameInstance->GetGameState() == EGameState::Playing && GameInstance->GetPlayingState() == EPlayingState::Bunker)
			{
				TArray<UItemInstanceBase*> Slots = GameInstance->GetStorageInventory();
				TArray<UItemInstanceBase*> NewSlots{};

				for (UItemInstanceBase* Slot : Slots)
				{
					UItemInstanceBase* Item = DuplicateObject<UItemInstanceBase>(Slot, this);
					NewSlots.Add(Item);
				}

				InventoryComponent->SetSlots(NewSlots);
			}
		}
	}
}

void AStorageContainerActor::PerformInteract(AActor* Interactor)
{
	UUIManager* UIManager = UUIManager::Get(Interactor);
	if (!UIManager)
		return;

	UUserWidget* Widget = UIManager->PushScreen(EUIScreen::Inventory, EUIInputMode::GameAndUI);
	if (UInventoryScreenWidget* InventoryScreen = Cast<UInventoryScreenWidget>(Widget))
	{
		InventoryScreen->OpenContainer(InventoryComponent, GetDisplayTitle(Interactor));
	}
	SetOpened();
}

void AStorageContainerActor::SetOpened()
{
	PromptData.HoldSeconds = 0.f;
	if (bIsOpened || !OpenedMesh || !StaticMeshComp)
		return;

	StaticMeshComp->SetStaticMesh(OpenedMesh);
	bIsOpened = true;
}

void AStorageContainerActor::SetItems(const TArray<UItemInstanceBase*>& Items)
{
	if (!InventoryComponent)
		return;

	for (UItemInstanceBase* Item : Items)
	{
		if (!IsValid(Item))
			continue;

		if (!InventoryComponent->AddItem(Item))
		{
			UE_LOG(LogTemp, Warning, TEXT("StorageContainerActor: 슬롯이 부족해서 아이템(%s)을 넣지 못했습니다."), *Item->GetName());
		}
	}
}
