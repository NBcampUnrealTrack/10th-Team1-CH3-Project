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
#include "Kismet/KismetSystemLibrary.h"
#include "Monster/ActorComponent/MonsterStatComponent.h"
#include "Monster/AiController/MonsterAIController.h"
#include "Monster/DataAssets/MonsterDataAsset.h"
#include "Monster/System/BFLMissileAttack.h"
#include "Monster/System/BFLMonsterStorageSpawn.h"
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

void AMonsterCharacter::MoveFlying(const FVector& TargetLocation)
{
	if (GetMonsterType() == EMonsterType::Fly ||
		GetMonsterType() == EMonsterType::Boss)
	{
		const FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
		AddMovementInput(Direction);
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

	FVector AttackPoint = GetMesh()->GetSocketLocation(*SocketName.ToString());
	FRotator AttackFocus = GetMesh()->GetSocketRotation(*SocketName.ToString());

	if (Effect)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
															Effect,
															AttackPoint,
															AttackFocus,
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
	if (!Target)
	{
		return ActualDamage;
	}
	if (MonsterStat->GetIsDead())
	{
		return ActualDamage;
	}

	MonsterController->PlantFlag(EFlag::TakeDamage, CurrentTime);
	MonsterController->SetTarget(Target);
	MonsterStat->ApplyProtect(ActualDamage, Target);

	DeathSequence(true);

	return ActualDamage;
}

EPointPatrolState AMonsterCharacter::NowPatrolState() const
{
	return PatrolState;
}

void AMonsterCharacter::ChangePatrolState()
{
	if (PatrolState == EPointPatrolState::Go)
	{
		PatrolState = EPointPatrolState::Return;
	}
	else
	{
		PatrolState = EPointPatrolState::Go;
	}
}

int AMonsterCharacter::GetPointX() const
{
	return PointRangeX;
}

int AMonsterCharacter::GetPointY() const
{
	return PointRangeY;
}

EPatrolType AMonsterCharacter::IsPatrolType() const
{
	return PatrolType;
}

void AMonsterCharacter::DeathSequence(bool Cast)
{
	if (!MonsterStat->GetIsDead() && Cast)
	{
		return;
	}
	if (Cast)
	{
		if (GetWorld())
		{
			if (ABOGameMode* GameMode = GetWorld()->GetAuthGameMode<ABOGameMode>())
			{
				GameMode->AddKilledMonster(GetMonsterID(), GetMonsterType());
			}
		}
	}

	OnDeleteMonster.Broadcast();

	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->MaxWalkSpeed = 0.0f;
		Movement->StopMovementImmediately();
	}

	GetWorld()->GetTimerManager().SetTimer(DeathMotionTimer,
										   this,
										   &AMonsterCharacter::EraseMonster,
										   1.40f,
										   false);
}

void AMonsterCharacter::EraseMonster()
{
	UBFLMonsterStorageSpawn::StorageSpawn(GetActorLocation(),
										  GetMonsterID(),
										  GetWorld());
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
	OnStatSetComplete.Broadcast();
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->MaxWalkSpeed = MonsterStat->GetWalkSpeed();
		Movement->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
		if (MonsterStat->GetMonsterType() == EMonsterType::Fly)
		{
			Movement->GravityScale = 0.0f;
			Movement->SetMovementMode(MOVE_Flying);
			Movement->DefaultLandMovementMode = MOVE_Flying;
		}
	}
}

// MonsterData

UMonsterDataAsset* AMonsterCharacter::GetMonsterData() const
{
	return MonsterData;
}

// MonsterStat

float AMonsterCharacter::GetFlyMax() const
{
	return MonsterStat->GetFlyMax();
}

float AMonsterCharacter::GetFlyMin() const
{
	return MonsterStat->GetFlyMin();
}

EMonsterType AMonsterCharacter::GetMonsterType() const
{
	return MonsterStat->GetMonsterType();
}

UMonsterStatComponent* AMonsterCharacter::GetMonsterStats() const
{
	return MonsterStat;
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

void AMonsterCharacter::OnMissileHit(TArray<FOverlapResult> Targets)
{
	MonsterStat->OnMissileHit(Targets);
}

// Mesh

FVector AMonsterCharacter::GetAttackPoint() const
{
	return GetMesh()->GetSocketLocation(*SocketName.ToString());
}

FRotator AMonsterCharacter::GetAttackRotator() const
{
	return GetMesh()->GetSocketRotation(*SocketName.ToString());
}
