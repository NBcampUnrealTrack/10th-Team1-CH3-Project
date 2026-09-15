#include "Projectiles/Throwables/ThrowableProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AThrowableProjectile::AThrowableProjectile()
{
	// StaticMesh 생성
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(GetRootComponent());

	StaticMesh->SetSimulatePhysics(false);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMesh->SetCollisionObjectType(ECC_WorldDynamic);
	StaticMesh->SetCollisionResponseToAllChannels(ECR_Block);              // 나머지 Block
	StaticMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore); // Visibility -> Ignore
	StaticMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);     // Camera -> Ignore
	StaticMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);       // Pawn -> Ignore

	// Collision 생성
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetupAttachment(StaticMesh);

	// ProjectileMovement 설정
	ProjectileMovement->UpdatedComponent = StaticMesh;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.3f;
	ProjectileMovement->Friction = 0.5f;
	ProjectileMovement->BounceVelocityStopSimulatingThreshold = 10.0f;
}

void AThrowableProjectile::Initalize(
	APawn* InInstigator,
	int32 InDamage,
	const float InRadius,
	const float InDelay,
	const FVector& Velocity,
	const float GravityScale)
{
	Super::Initialize(
		InInstigator,
		InDamage,
		Velocity,
		GravityScale);

	// Instigator와의 충돌 무시
	Collision->IgnoreActorWhenMoving(InInstigator, true);

	// 반경 & 딜레이 저장
	Radius = InRadius;
	Delay = InDelay;

	// 콜리전 반경 적용
	Collision->SetSphereRadius(InRadius);

	// 타이머 활성화
	GetWorldTimerManager()
		.SetTimer(
			ActivateTimerHandle,
			this,
			&AThrowableProjectile::Activate,
			InDelay,
			false);
}

void AThrowableProjectile::Activate()
{
}
