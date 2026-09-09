#include "Items/Actors/ItemPickupBase.h"

AItemPickupBase::AItemPickupBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// 스태틱 메시 컴포넌트 생성
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(StaticMeshComp);

	// 물리 & 중력 활성화
	StaticMeshComp->SetSimulatePhysics(true);
	StaticMeshComp->SetEnableGravity(true);

	// 콜리전 설정
	StaticMeshComp->SetCollisionResponseToAllChannels(ECR_Block);               // 나머지 Block
	StaticMeshComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);  // Visibility -> Ignore
	StaticMeshComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);      // Camera -> Ignore
	StaticMeshComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);       // Pawn -> Overlap

	ItemInstanceClass = nullptr;
	ItemInstance = nullptr;
}

void AItemPickupBase::Initialize(UItemInstanceBase* InItemInstance)
{
	ItemInstance = InItemInstance;
}

UItemInstanceBase* AItemPickupBase::GetItemInstance() const
{
	return ItemInstance;
}

void AItemPickupBase::BeginPlay()
{
	Super::BeginPlay();
}
