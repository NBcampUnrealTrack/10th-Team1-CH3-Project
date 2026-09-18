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

	FMonsterStatInfo* MonsterStastInfo = MonsterData->StatTable->FindRow<FMonsterStatInfo>(Monster->GetMonsterID(), TEXT("MonsterID Serching"));
	if (!MonsterStastInfo)
	{
		SetMonsterID("Gunner");
		MonsterStastInfo = MonsterData->StatTable->FindRow<FMonsterStatInfo>(Monster->GetMonsterID(), TEXT("GunnerID Serching"));
	}

	// Health Info
	MaxHealth = MonsterStastInfo->MaxHealth;
	CurHealth = MonsterStastInfo->CurHealth;
	MaxShield = MonsterStastInfo->MaxShield;
	CurShield = MonsterStastInfo->CurShield;
	ShieldDelayTime = MonsterStastInfo->ShieldDelayTime;
	ShieldRegenTime = MonsterStastInfo->ShieldRegenTime;
	ShieldRegenAmount = MonsterStastInfo->ShieldRegenAmount;

	// Attack Info
	AttackDamage = MonsterStastInfo->AttackDamage;
	RapidCount = MonsterStastInfo->RapidCount;
	RapidDelay = MonsterStastInfo->RapidDelay;
	AttackDelay = MonsterStastInfo->AttackDelay;
	AttackRange = MonsterStastInfo->AttackRange;
	BulletSpeed = MonsterStastInfo->BulletSpeed;

	// Another Info
	Protect = MonsterStastInfo->Protect;
	Intelligence = MonsterStastInfo->Intelligence;
	WalkSpeed = MonsterStastInfo->WalkSpeed;
	SprintSpeed = MonsterStastInfo->SprintSpeed;

	// Monster key Info
	MonsterType = MonsterStastInfo->MonsterType;
}
