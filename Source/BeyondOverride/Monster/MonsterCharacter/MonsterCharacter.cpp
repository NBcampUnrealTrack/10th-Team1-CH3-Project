// 26/09/09 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/MonsterCharacter/MonsterCharacter.h"

// Add include
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/ActorComponent.h"
#include "Components/CapsuleComponent.h"
#include "DataTables/Monster/MonsterInfo.h"
#include "Engine/DataTable.h"
#include "GameFlow/BOGameMode.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Monster/ActorComponent/MonsterStatComponent.h"
#include "Monster/AiController/MonsterAIController.h"
#include "Monster/DataAssets/MonsterDataAsset.h"
#include "Particles/ParticleSystemComponent.h"
#include "Player/Character/BOCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMonsterCharacter::AMonsterCharacter()
{

	MonsterStat = CreateDefaultSubobject<UMonsterStatComponent>(TEXT("MonsterStat"));

	AIControllerClass = AMonsterAIController::StaticClass();

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	static ConstructorHelpers::FObjectFinder<UMonsterDataAsset> DataAssetFinder(TEXT("/Game/Blueprints/Monster/DataAssets/DA_MonstersInfo.DA_MonstersInfo"));

	if (DataAssetFinder.Succeeded())
	{
		MonsterData = DataAssetFinder.Object;
	}
}

void AMonsterCharacter::FocusSetUp(bool data)
{
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		bUseControllerRotationYaw = data;
		bUseControllerRotationRoll = data;
		Movement->bOrientRotationToMovement = data;

		if (MonsterStat->GetMonsterType() == EMonsterType::Fly)
		{
			bUseControllerRotationPitch = data;
		}
	}
}

void AMonsterCharacter::MonsterAttack()
{
	AMonsterAIController* MonsterController = Cast<AMonsterAIController>(GetController());
	if (!MonsterController)
	{
		return;
	}

	MonsterController->StateChange(EMonsterState::Attack, 0.3f);
	MonsterStat->Attack();

	UParticleSystemComponent* Particle = nullptr;

	if (Effect)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
															Effect,
															GetMesh()->GetSocketLocation(*SocketName.ToString()),
															GetMesh()->GetSocketRotation(*SocketName.ToString()),
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
}

float AMonsterCharacter::TakeDamage(float DamageAmount,
									FDamageEvent const& DamageEvent,
									AController* EventInstigator,
									AActor* DamageCauser)
{
	const float CurrentTime = GetWorld()->GetTimeSeconds();

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ABOCharacter* Target = nullptr;

	AMonsterAIController* MonsterController = Cast<AMonsterAIController>(GetController());
	if (!MonsterController)
	{
		return ActualDamage;
	}

	if (EventInstigator)
	{
		Target = Cast<ABOCharacter>(EventInstigator->GetPawn());
	}

	MonsterController->PlantFlag(EFlag::TakeDamage, CurrentTime);
	MonsterController->SetTarget(Target);
	MonsterStat->ApplyProtect(ActualDamage, Target);

	DeathSequence();

	return ActualDamage;
}

void AMonsterCharacter::DeathSequence()
{

	if (!MonsterStat->GetIsDead())
	{
		return;
	}

	if (GetWorld())
	{
		if (ABOGameMode* GameMode = GetWorld()->GetAuthGameMode<ABOGameMode>())
		{
			GameMode->AddKilledMonster(GetMonsterID());
		}
	}

	Destroy();
}

void AMonsterCharacter::SetUpMesh()
{

	if (!MonsterData)
	{
		return;
	}

	if (!MonsterData->MeshTable)
	{
		return;
	}

	FMonsterInfo* MonsterInfo = MonsterData->MeshTable->FindRow<FMonsterInfo>(GetMonsterID(), TEXT("MonsterID Serching"));

	if (!MonsterInfo)
	{
		SetMonsterID("Gunner");
		MonsterInfo = MonsterData->MeshTable->FindRow<FMonsterInfo>(GetMonsterID(), TEXT("GunnerID Serching"));
	}

	Effect = MonsterInfo->MonsterAttackEffect;
	SocketName = MonsterInfo->MonsterAttackSocket;

	if (MonsterInfo->MonsterSkeletal)
	{
		GetCapsuleComponent()->SetCapsuleSize(MonsterInfo->CapsuleRadius,
											  MonsterInfo->CapsuleHalfHeight);
		GetMesh()->SetSkeletalMesh(MonsterInfo->MonsterSkeletal);
		GetMesh()->SetRelativeScale3D(MonsterInfo->SkeletalScale);
		GetMesh()->SetRelativeLocation(MonsterInfo->SkeletalLocation);
		GetMesh()->SetRelativeRotation(MonsterInfo->SkeletalRotation);
	}

	if (MonsterInfo->MonsterAnimInstance)
	{
		GetMesh()->SetAnimInstanceClass(MonsterInfo->MonsterAnimInstance);
	}
}

void AMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMonsterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetUpMesh();
	MonsterStat->StatSetup();
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->MaxWalkSpeed = MonsterStat->GetWalkSpeed() * 1;
		Movement->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	}
}

void AMonsterCharacter::SetMonsterID(FName ID)
{
	MonsterStat->SetMonsterID(ID);
}

FName AMonsterCharacter::GetMonsterID() const
{
	return MonsterStat->GetMonsterID();
}

float AMonsterCharacter::GetAttackRange() const
{
	return MonsterStat->GetAttackRange();
}

bool AMonsterCharacter::IsDelay()
{
	return MonsterStat->IsDelay();
}

FVector AMonsterCharacter::GetAttackPoint() const
{
	return GetMesh()->GetSocketLocation(*SocketName.ToString());
}

이동 중 벽에 끼는 현상 - 두꺼운 트레이스를 캡슐 크기로 사용하여 해당 위치에 도착 가능한지 판정 후 해당 내용을 기반으로 위치 선정
애니메이션 - 조사를 통한 해결
근접 공격 구현,
사망 애니메이션,


IDamageAble::TakeDamage(float InDamage)

적, 플레이어 -> IDamageAble::TakeDamage 구현 함

=> 체력 관련 로직 처리

보물 상자, 숨겨진 통로를 막은 벽 -> IDamageAble::TakeDamage 구현됨

=> 바로 부셔짐 -> 체력이 없으니깐

단순 벽, 바닥, 또는 부술 수 없는 사물 -> IDamageAble::TakeDamage 구현 안됨

=> IDamageAble가 없으니 Cast시 Null 반환 되어서 그 이후로 진행 불가

MOVE_Flying-캐릭터 컴포넌트에 기초 기능










// 26/09/09 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/MonsterCharacter/MonsterCharacter.h"

// Add include
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/ActorComponent.h"
#include "Components/CapsuleComponent.h"
#include "DataTables/Monster/MonsterInfo.h"
#include "Engine/DataTable.h"
#include "GameFlow/BOGameMode.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Monster/ActorComponent/MonsterStatComponent.h"
#include "Monster/AiController/MonsterAIController.h"
#include "Monster/DataAssets/MonsterDataAsset.h"
#include "Particles/ParticleSystemComponent.h"
#include "Player/Character/BOCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMonsterCharacter::AMonsterCharacter()
{

	MonsterStat = CreateDefaultSubobject<UMonsterStatComponent>(TEXT("MonsterStat"));

	AIControllerClass = AMonsterAIController::StaticClass();

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	static ConstructorHelpers::FObjectFinder<UMonsterDataAsset> DataAssetFinder(TEXT("/Game/Blueprints/Monster/DataAssets/DA_MonstersInfo.DA_MonstersInfo"));

	if (DataAssetFinder.Succeeded())
	{
		MonsterData = DataAssetFinder.Object;
	}
}

void AMonsterCharacter::FocusSetUp(bool data)
{
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		bUseControllerRotationYaw = data;
		bUseControllerRotationRoll = data;
		Movement->bOrientRotationToMovement = data;

		if (MonsterStat->GetMonsterType() == EMonsterType::Fly)
		{
			bUseControllerRotationPitch = data;
		}
	}
}

void AMonsterCharacter::MonsterAttack()
{
	AMonsterAIController* MonsterController = Cast<AMonsterAIController>(GetController());
	if (!MonsterController)
	{
		return;
	}

	MonsterController->StateChange(EMonsterState::Attack, 0.3f);
	MonsterStat->Attack();

	UParticleSystemComponent* Particle = nullptr;

	if (Effect)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
															Effect,
															GetMesh()->GetSocketLocation(*SocketName.ToString()),
															GetMesh()->GetSocketRotation(*SocketName.ToString()),
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
}

float AMonsterCharacter::TakeDamage(float DamageAmount,
									FDamageEvent const& DamageEvent,
									AController* EventInstigator,
									AActor* DamageCauser)
{
	const float CurrentTime = GetWorld()->GetTimeSeconds();

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ABOCharacter* Target = nullptr;

	AMonsterAIController* MonsterController = Cast<AMonsterAIController>(GetController());
	if (!MonsterController)
	{
		return ActualDamage;
	}

	if (EventInstigator)
	{
		Target = Cast<ABOCharacter>(EventInstigator->GetPawn());
	}

	MonsterController->PlantFlag(EFlag::TakeDamage, CurrentTime);
	MonsterController->SetTarget(Target);
	MonsterStat->ApplyProtect(ActualDamage, Target);

	DeathSequence();

	return ActualDamage;
}

void AMonsterCharacter::DeathSequence()
{

	if (!MonsterStat->GetIsDead())
	{
		return;
	}

	if (GetWorld())
	{
		if (ABOGameMode* GameMode = GetWorld()->GetAuthGameMode<ABOGameMode>())
		{
			GameMode->AddKilledMonster(GetMonsterID());
		}
	}

	Destroy();
}

void AMonsterCharacter::SetUpMesh()
{

	if (!MonsterData)
	{
		return;
	}

	if (!MonsterData->MeshTable)
	{
		return;
	}

	FMonsterInfo* MonsterInfo = MonsterData->MeshTable->FindRow<FMonsterInfo>(GetMonsterID(), TEXT("MonsterID Serching"));

	if (!MonsterInfo)
	{
		SetMonsterID("Gunner");
		MonsterInfo = MonsterData->MeshTable->FindRow<FMonsterInfo>(GetMonsterID(), TEXT("GunnerID Serching"));
	}

	Effect = MonsterInfo->MonsterAttackEffect;
	SocketName = MonsterInfo->MonsterAttackSocket;

	if (MonsterInfo->MonsterSkeletal)
	{
		GetCapsuleComponent()->SetCapsuleSize(MonsterInfo->CapsuleRadius,
											  MonsterInfo->CapsuleHalfHeight);
		GetMesh()->SetSkeletalMesh(MonsterInfo->MonsterSkeletal);
		GetMesh()->SetRelativeScale3D(MonsterInfo->SkeletalScale);
		GetMesh()->SetRelativeLocation(MonsterInfo->SkeletalLocation);
		GetMesh()->SetRelativeRotation(MonsterInfo->SkeletalRotation);
	}

	if (MonsterInfo->MonsterAnimInstance)
	{
		GetMesh()->SetAnimInstanceClass(MonsterInfo->MonsterAnimInstance);
	}
}

void AMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMonsterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetUpMesh();
	MonsterStat->StatSetup();
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->MaxWalkSpeed = MonsterStat->GetWalkSpeed() * 1;
		Movement->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	}
}

void AMonsterCharacter::SetMonsterID(FName ID)
{
	MonsterStat->SetMonsterID(ID);
}

FName AMonsterCharacter::GetMonsterID() const
{
	return MonsterStat->GetMonsterID();
}

float AMonsterCharacter::GetAttackRange() const
{
	return MonsterStat->GetAttackRange();
}

bool AMonsterCharacter::IsDelay()
{
	return MonsterStat->IsDelay();
}

FVector AMonsterCharacter::GetAttackPoint() const
{
	return GetMesh()->GetSocketLocation(*SocketName.ToString());
}






// 26/09/14 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/ActorComponent/MonsterStatComponent.h"

// Add include
#include "DataTables/Monster/MonsterStatInfo.h"
#include "Kismet/GameplayStatics.h"
#include "Monster/ActorComponent/ContinuousStateComponent.h"
#include "Monster/AiController/MonsterAIController.h"
#include "Monster/DataAssets/MonsterDataAsset.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"
#include "Monster/System/BalisticTrace.h"
#include "Player/Character/BOCharacter.h"

UMonsterStatComponent::UMonsterStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UMonsterDataAsset> DataAssetFinder(TEXT("/Game/Blueprints/Monster/DataAssets/DA_MonstersInfo.DA_MonstersInfo"));

	if (DataAssetFinder.Succeeded())
	{
		MonsterData = DataAssetFinder.Object;
	}
}

EMonsterType UMonsterStatComponent::GetMonsterType() const
{
	return MonsterType;
}

float UMonsterStatComponent::GetWalkSpeed() const
{
	return WalkSpeed;
}

float UMonsterStatComponent::GetSprintSpeed() const
{
	return SprintSpeed;
}

void UMonsterStatComponent::SetAttackRange(float Range)
{
	AttackRange = Range;
}

float UMonsterStatComponent::GetAttackRange() const
{
	return AttackRange;
}

FVector UMonsterStatComponent::GetAttackPoint() const
{
	AMonsterCharacter* Owner = Cast<AMonsterCharacter>(GetOwner());
	if (!Owner)
	{
		return FVector::ZeroVector;
	}
	return Owner->GetAttackPoint();
}

void UMonsterStatComponent::SetMonsterID(FName ID)
{
	MonsterID = ID;
}

FName UMonsterStatComponent::GetMonsterID() const
{
	return MonsterID;
}

void UMonsterStatComponent::Attack()
{

	if (IsDelay())
	{
		return;
	}

	if (MonsterType == EMonsterType::Range)
	{
		AMonsterCharacter* Owner = Cast<AMonsterCharacter>(GetOwner());
		if (!Owner)
		{
			return;
		}
		AMonsterAIController* AIController = Cast<AMonsterAIController>(Owner->GetController());
		if (!AIController)
		{
			return;
		}
		ABOCharacter* Target = AIController->GetTarget();
		if (!Target)
		{
			return;
		}

		FVector BulletDirection = (Target->GetActorLocation() - GetAttackPoint()).GetSafeNormal();

		UBalisticTrace* NewBalisticTrace = NewObject<UBalisticTrace>(this);

		NewBalisticTrace->OnBalisticHit.AddUObject(this, &UMonsterStatComponent::OnBalisticHit);

		NewBalisticTrace->BalisticStart(Owner,
										GetAttackPoint(),
										BulletDirection,
										AttackDelay,
										BulletSpeed);
	}

	CallAttackLock();
}

void UMonsterStatComponent::ApplyProtect(int32 getdamage, AActor* DamageCauser)
{
	TakeDamage(FMath::Max(1, getdamage - Protect), DamageCauser);
}

bool UMonsterStatComponent::IsDelay() const
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	return TimerManager.IsTimerActive(AttackLock);
}

void UMonsterStatComponent::CallAttackLock()
{
	if (IsDelay())
	{
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(AttackLock,
										   FTimerDelegate(),
										   AttackDelay,
										   false);
}

void UMonsterStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMonsterStatComponent::OnBalisticHit(AActor* Target)
{
	AMonsterCharacter* Owner = Cast<AMonsterCharacter>(GetOwner());
	if (!Owner)
	{
		return;
	}
	ABOCharacter* PlayerCharacter = Cast<ABOCharacter>(Target);
	if (!PlayerCharacter)
	{
		return;
	}
	UGameplayStatics::ApplyDamage(Target,
								  AttackDamage,
								  Owner->GetController(),
								  Owner,
								  UDamageType::StaticClass());
}

void UMonsterStatComponent::StatSetup()
{
	if (!MonsterData)
	{
		return;
	}
	AMonsterCharacter* Monster = Cast<AMonsterCharacter>(GetOwner());
	if (!Monster)
	{
		return;
	}

	FMonsterStatInfo* MonsterStatInfo = MonsterData->StatTable->FindRow<FMonsterStatInfo>(Monster->GetMonsterID(), TEXT("MonsterID Serching"));
	if (!MonsterStatInfo)
	{
		SetMonsterID("Gunner");
		MonsterStatInfo = MonsterData->StatTable->FindRow<FMonsterStatInfo>(Monster->GetMonsterID(), TEXT("GunnerID Serching"));
	}

	// Health Info
	MaxHealth = MonsterStatInfo->MaxHealth;
	CurHealth = MonsterStatInfo->CurHealth;
	MaxShield = MonsterStatInfo->MaxShield;
	CurShield = MonsterStatInfo->CurShield;
	ShieldDelayTime = MonsterStatInfo->ShieldDelayTime;
	ShieldRegenTime = MonsterStatInfo->ShieldRegenTime;
	ShieldRegenAmount = MonsterStatInfo->ShieldRegenAmount;

	// Attack Info
	AttackDamage = MonsterStatInfo->AttackDamage;
	RapidCount = MonsterStatInfo->RapidCount;
	RapidDelay = MonsterStatInfo->RapidDelay;
	AttackDelay = MonsterStatInfo->AttackDelay;
	AttackRange = MonsterStatInfo->AttackRange;
	BulletSpeed = MonsterStatInfo->BulletSpeed;

	// Another Info
	Protect = MonsterStatInfo->Protect;
	Intelligence = MonsterStatInfo->Intelligence;
	WalkSpeed = MonsterStatInfo->WalkSpeed;
	SprintSpeed = MonsterStatInfo->SprintSpeed;

	// Monster key Info
	MonsterType = MonsterStatInfo->MonsterType;
}
