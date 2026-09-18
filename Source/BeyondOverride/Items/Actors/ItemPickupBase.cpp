#include "Items/Actors/ItemPickupBase.h"

#include "DataTables/Items/ItemDataRow.h"
#include "Factory/ItemFactory.h"
#include "Items/Objects/ItemInstanceBase.h"

AItemPickupBase::AItemPickupBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// 스태틱 메시 컴포넌트 생성
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(StaticMeshComp);

	// 물리 & 중력 활성화
	StaticMeshComp->SetSimulatePhysics(true);
	StaticMeshComp->SetEnableGravity(true);

	// CCD - 빠른 이동 시 관통 방지
	StaticMeshComp->SetUseCCD(true);

	// 오브젝트 타입 설정
	StaticMeshComp->SetCollisionObjectType(ECC_PhysicsBody);

	// 콜리전 채널 설정
	StaticMeshComp->SetCollisionResponseToAllChannels(ECR_Block);              // 나머지 Block
	StaticMeshComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore); // Visibility -> Ignore
	StaticMeshComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);     // Camera -> Ignore
	StaticMeshComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);      // Pawn -> Overlap

	ItemInstance = nullptr;

	StackCount = 1;

	PromptData.ActionText = FText::FromString(TEXT("획득"));
}

void AItemPickupBase::Initialize(UItemInstanceBase* InItemInstance)
{
	// InItemInstance 유효성 검사
	if (!InItemInstance)
	{
		return;
	}

	// ItemData 확인
	const FItemDataRow* ItemData = ItemInstance->GetItemData();
	if (!ItemData)
	{
		return;
	}

	// 인스턴스 저장
	ItemInstance = InItemInstance;
	StackCount = InItemInstance->GetStackCount();

	// 메시 설정
	if (UStaticMesh* Mesh = ItemData->ItemPickupMesh)
	{
		StaticMeshComp->SetStaticMesh(Mesh);
	}
}

UItemInstanceBase* AItemPickupBase::GetItemInstance() const
{
	return ItemInstance;
}

void AItemPickupBase::BeginPlay()
{
	Super::BeginPlay();

	// 아이템 인스턴스 생성 - 저장된 인스턴스 없는 경우
	if (!ItemInstance)
	{
		ItemInstance = FItemFactory::CreateItemInstance(this, ItemID);
		ItemInstance->SetStackCount(StackCount); // 개수 설정
		Initialize(ItemInstance);
	}
}

FText AItemPickupBase::GetDisplayTitle(AActor* Interactor) const
{
	if (ItemInstance)
	{
		if (const FItemDataRow* ItemData = ItemInstance->GetItemData())
		{
			return ItemData->DisplayName;
		}
	}

	return Super::GetDisplayTitle(Interactor);
}
