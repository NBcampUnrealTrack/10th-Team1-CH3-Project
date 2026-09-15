// 26/09/09 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/MonsterCharacter/MonsterCharacter.h"

// Add include
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Monster/ActorComponent/AttackDataComponent.h"
#include "Monster/ActorComponent/MonsterStatComponent.h"
#include "Monster/ActorComponent/StateComponent.h"
#include "Monster/AiController/MonsterAIController.h"
#include "Monster/DataAssets/MonsterDataAsset.h"
#include "Monster/DataTable/MonsterInfo.h"
#include "Particles/ParticleSystemComponent.h"
#include "Player/ActorComponent/StatComponent.h"
#include "Player/Character/BOCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMonsterCharacter::AMonsterCharacter()
{

	MonsterStat = CreateDefaultSubobject<UMonsterStatComponent>(TEXT("MonsterStat"));

	StateComponent = CreateDefaultSubobject<UStateComponent>(TEXT("StateComponent"));

	AttackDataComponent = CreateDefaultSubobject<UAttackDataComponent>(TEXT("AttackDataComponent"));

	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("HPStatComponent"));

	AIControllerClass = AMonsterAIController::StaticClass();

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	UCharacterMovementComponent* Movement = GetCharacterMovement();

	if (Movement)
	{
		Movement->MaxWalkSpeed = WalkSpeed * 1;
		Movement->bOrientRotationToMovement = true;
		Movement->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	}

	static ConstructorHelpers::FObjectFinder<UMonsterDataAsset> DataAssetFinder(TEXT("/Game/Blueprints/Monster/DataAssets/DA_MonstersInfo.DA_MonstersInfo"));

	if (DataAssetFinder.Succeeded())
	{
		MonsterData = DataAssetFinder.Object;
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

	if (!StatComponent->GetIsDead())
	{
		return;
	}
	// ABOGameMode::AddKilledMonster(GetMonsterID());

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
		SetMonsterID("Wraith");
		MonsterInfo = MonsterData->MeshTable->FindRow<FMonsterInfo>(GetMonsterID(), TEXT("MonsterID Serching"));
	}

	Effect = MonsterInfo->MonsterAttackEffect;
	SocketName = MonsterInfo->MonsterAttackSocket;

	if (MonsterInfo->MonsterSkeletal)
	{
		GetMesh()->SetSkeletalMesh(MonsterInfo->MonsterSkeletal);
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
