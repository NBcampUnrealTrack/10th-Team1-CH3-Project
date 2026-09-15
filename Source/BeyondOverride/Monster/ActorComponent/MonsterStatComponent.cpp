// 26/09/14 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/ActorComponent/MonsterStatComponent.h"

// Add include
#include "Kismet/GameplayStatics.h"
#include "Monster/ActorComponent/ContinuousStateComponent.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"
#include "Monster/System/BalisticTrace.h"

UMonsterStatComponent::UMonsterStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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
		const AActor* Owner = GetOwner();
		if (!Owner)
		{
			return;
		}
		UBalisticTrace* NewBalisticTrace = NewObject<UBalisticTrace>(this);

		NewBalisticTrace->OnBalisticHit.AddUObject(this, &UMonsterStatComponent::OnBalisticHit);

		NewBalisticTrace->BalisticStart(RangeAttackResult,
										Owner,
										GetAttackPoint(),
										FVector::ZeroVector,
										AttackDelay,
										BulletSpeed);
	}

	CallAttackLock();
}

void UMonsterStatComponent::ApplyProtect(int32 getdamage)
{
	TakeDamage(FMath::Max(1, getdamage - Protect));
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

void UMonsterStatComponent::OnBalisticHit()
{
	AMonsterCharacter* Owner = Cast<AMonsterCharacter>(GetOwner());
	if (!Owner)
	{
		return;
	}

	UGameplayStatics::ApplyDamage(RangeAttackResult.GetActor(),
								  AttackDamage,
								  Owner->GetController(),
								  Owner,
								  UDamageType::StaticClass());
}

/*
 */
