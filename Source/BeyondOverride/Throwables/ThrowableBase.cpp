#include "Throwables/ThrowableBase.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AThrowableBase::AThrowableBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// SkeletalMesh 생성
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SetRootComponent(SkeletalMesh);

	// Physics 활성화
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SkeletalMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore); // Pawn 충돌 무시
	SkeletalMesh->SetSimulatePhysics(true);
	SkeletalMesh->SetEnableGravity(true);

	// IgnoreInstigatorCollisionTime 기본값 설정
	IgnoreInstigatorCollisionTime = 0.1f;
}

void AThrowableBase::Throw(
	APawn* InInstigator,
	const FRotator& Rotation,
	const float Force)
{
	if (!IsValid(SkeletalMesh) || !IsValid(InInstigator))
	{
		return;
	}

	// Instigator 등록
	SetInstigator(InInstigator);

	// Instigator와의 충돌 무시
	SkeletalMesh->IgnoreActorWhenMoving(InInstigator, true);

	// 일정 시간 후 Instigator와의 충돌 허용
	GetWorldTimerManager().SetTimer(
		IgnoreInstigatorCollisionTimerHandle,
		[this]()
		{
			if (IsValid(SkeletalMesh))
			{
				SkeletalMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block); // Pawn 충돌 활성화
				SkeletalMesh->IgnoreActorWhenMoving(GetInstigator(), false);
			}
		},
		IgnoreInstigatorCollisionTime,
		false);

	// Throw 수행
	SkeletalMesh->AddImpulse(Rotation.Vector() * Force, NAME_None, true);
}

void AThrowableBase::Activate()
{
	PlayActivationEffects(GetActorLocation(), GetActorRotation());

	Destroy();
}

void AThrowableBase::PlayActivationEffects(const FVector& Location, const FRotator& Rotation)
{
	// 피격 파티클 재생
	if (ActivationParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ActivationParticle,
			Location,
			Rotation);
	}

	// 피격 사운드 재생
	if (ActivationSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			ActivationSound,
			Location);
	}
}
