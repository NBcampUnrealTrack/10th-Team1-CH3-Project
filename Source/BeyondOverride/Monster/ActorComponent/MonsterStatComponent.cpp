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
