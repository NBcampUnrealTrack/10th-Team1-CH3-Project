// 26/09/09 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/MonsterCharacter/MonsterCharacter.h"

// Add include
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/ActorComponent.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Monster/ActorComponent/AttackDataComponent.h"
#include "Monster/ActorComponent/StateComponent.h"
#include "Monster/AiController/MonsterAIController.h"
#include "Particles/ParticleSystemComponent.h"
#include "Player/ActorComponent/StatComponent.h"
#include "Player/Character/BOCharacter.h"

AMonsterCharacter::AMonsterCharacter()
{

	// 상태 데이터 컴포넌트
	StateComponent = CreateDefaultSubobject<UStateComponent>(TEXT("StateComponent"));

	// 공격 데이터 컴포넌트
	AttackDataComponent = CreateDefaultSubobject<UAttackDataComponent>(TEXT("AttackDataComponent"));

	// 체력 데이터 컴포넌트
	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("HPStatComponent"));

	// 캐릭터에 대한 기본 AIController 지정
	AIControllerClass = AMonsterAIController::StaticClass();

	// Level 배치 혹은 Spawned 시 Chracter를 Possess하도록 설정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 캐릭터의 무브먼트 정보 가져오기
	UCharacterMovementComponent* Movement = GetCharacterMovement();

	if (Movement)
	{

		// Character 이동속도 설정
		if (MonsterType == EMonsterType::Special)
		{
			Movement->MaxWalkSpeed = WalkSpeed * 0.6;
		}
		// Character 이동속도 설정
		if (MonsterType == EMonsterType::Range)
		{
			Movement->MaxWalkSpeed = WalkSpeed * 0.8;
		}
		// Character 이동속도 설정
		if (MonsterType == EMonsterType::Melee)
		{
			Movement->MaxWalkSpeed = WalkSpeed * 1;
		}

		// Character 이동 방향으로 바라보기 설정
		Movement->bOrientRotationToMovement = true;
		// Character 이동 방향으로 회전하는 속도 설정
		Movement->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	}
}

void AMonsterCharacter::MonsterAttack()
{
	if (MonsterType == EMonsterType::Range)
	{
		CallBallistic();
	}
}

UStateComponent* AMonsterCharacter::GetState() const
{
	return StateComponent;
}

UAttackDataComponent* AMonsterCharacter::GetAttackData() const
{
	return AttackDataComponent;
}

float AMonsterCharacter::TakeDamage(float DamageAmount,
									FDamageEvent const& DamageEvent,
									AController* EventInstigator,
									AActor* DamageCauser)
{
	// 기본 데미지 처리 로직 호출 (필수는 아님)
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	int32 GetDamage = FMath::Max(1.0f, ActualDamage - GetAttackData()->GetProtect());
	ABOCharacter* Target = nullptr;

	if (EventInstigator)
	{
		Target = Cast<ABOCharacter>(EventInstigator->GetPawn());
	}

	StatComponent->TakeDamage(GetDamage);

	if (Target)
	{
		GetState()->SetTarget(Target);
		GetState()->CallGetDamage();
	}
	DeathSequence();

	return ActualDamage;
}

void AMonsterCharacter::DeathSequence()
{

	if (!StatComponent->GetIsDead())
	{
		return;
	}

	Destroy();
}

void AMonsterCharacter::CallBallistic()
{

	if (Ballistic.bHit || Ballistic.EndCount * Ballistic.FlyTime >= GetAttackData()->GetAttackDelay())
	{

		if (Ballistic.bHit)
		{
			UGameplayStatics::ApplyDamage(Ballistic.HitResult.GetActor(),
										  AttackDataComponent->GetAttackDamage(),
										  GetController(),
										  this,
										  UDamageType::StaticClass());
		}

		Ballistic.BulletLocation = FVector::ZeroVector;
		Ballistic.BulletDirection = FVector::ZeroVector;
		Ballistic.StartLocation = FVector::ZeroVector;
		Ballistic.EndLocation = FVector::ZeroVector;
		Ballistic.EndCount = 0;
		Ballistic.bHit = false;
		return;
	}
	if (Ballistic.StartLocation == FVector::ZeroVector)
	{

		Ballistic.BulletLocation = GetMesh()->GetSocketLocation(TEXT("Muzzle_01"));
		Ballistic.BulletDirection = (GetAttackData()->GetTargetLocation() - Ballistic.BulletLocation).GetSafeNormal();
		UParticleSystemComponent* Particle = nullptr;

		if (FireParticle)
		{
			Particle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
																FireParticle,
																Ballistic.BulletLocation,
																GetMesh()->GetSocketRotation(TEXT("Muzzle_01")),
																true);
			if (Particle)
			{
				FTimerHandle DestroyParticleTimerHandle;
				TWeakObjectPtr<UParticleSystemComponent> WeakParticle = Particle;

				GetWorld()->GetTimerManager().SetTimer(
					DestroyParticleTimerHandle,
					[WeakParticle]()
					{
						if (WeakParticle.IsValid())
						{
							WeakParticle->DestroyComponent();
						}
					},
					2.0f,
					false);
			}
		}

		Ballistic.StartLocation = Ballistic.BulletLocation;
	}
	else
	{
		Ballistic.StartLocation = Ballistic.EndLocation;
	}

	FVector InitialVelocity = Ballistic.BulletDirection * Ballistic.BulletSpeed;

	Ballistic.EndLocation = Ballistic.StartLocation + InitialVelocity * Ballistic.FlyTime + 0.5f * Ballistic.Gravity * Ballistic.FlyTime * Ballistic.FlyTime;
	Ballistic.EndCount = Ballistic.EndCount + 1;

	Ballistic.bHit = GetWorld()->LineTraceSingleByObjectType(Ballistic.HitResult,
															 Ballistic.StartLocation,
															 Ballistic.EndLocation,
															 Ballistic.TraceParams,
															 Ballistic.QueryParams);

	GetWorld()->GetTimerManager().SetTimer(Ballistic.Update,
										   this,
										   &AMonsterCharacter::CallBallistic,
										   Ballistic.FlyTime,
										   false);
}

void AMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();
	Ballistic.TraceParams.AddObjectTypesToQuery(ECC_Pawn);
	Ballistic.TraceParams.AddObjectTypesToQuery(ECC_WorldStatic);
	Ballistic.QueryParams.AddIgnoredActor(this);
}

void AMonsterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
