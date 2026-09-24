// 26/09/14 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/ActorComponent/MonsterStatComponent.h"

// Add include
#include "DataTables/Monster/MonsterStatInfo.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Monster/AiController/MonsterAIController.h"
#include "Monster/DataAssets/MonsterDataAsset.h"
#include "Monster/Enums/InfoEnums.h"
#include "Monster/Enums/StateEnums.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"
#include "Monster/System/BFLMeleeAttack.h"
#include "Monster/System/BFLMissileAttack.h"
#include "Monster/System/BFLSoundEvent.h"
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

		UBFLSoundEvent::SoundPlay(Owner->GetAttackPoint(), "Rifle", 3500.0f, 1.2f, 2.0f, false, GetWorld());

		UBalisticTrace* NewBalisticTrace = NewObject<UBalisticTrace>(this);

		NewBalisticTrace->OnBalisticHit.AddUObject(this, &UMonsterStatComponent::OnBalisticHit);

		NewBalisticTrace->BalisticStart(Owner,
										GetAttackPoint(),
										BulletDirection,
										AttackDelay,
										BulletSpeed);
	}
	else if (MonsterType == EMonsterType::Melee)
	{
		ACharacter* Owner = Cast<ACharacter>(GetOwner());
		if (!Owner)
		{
			return;
		}

		AMonsterAIController* AIController = Cast<AMonsterAIController>(Owner->GetController());
		if (!AIController)
		{
			return;
		}

		AActor* Target = UBFLMeleeAttack::DashAttack(Owner, AIController->GetTarget(), GetAttackRange());

		if (Target)
		{

			ABOCharacter* PlayerCharacter = Cast<ABOCharacter>(Target);
			if (!PlayerCharacter)
			{
				return;
			}
			DamageLogic(Target, AttackDamage);
		}
	}
	else if (MonsterType == EMonsterType::Special)
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

		UBFLSoundEvent::SoundPlay(Owner->GetAttackPoint(), "Missile", 5500.0f, 1.0f, 4.0f, false, GetWorld());
		FVector Delta = Target->GetActorLocation() - Owner->GetAttackPoint();

		float HorizontalDistance = FVector2D(Delta.X, Delta.Y).Size();
		float HeightDifference = Delta.Z;

		float Gravity = FMath::Abs(GetWorld()->GetGravityZ());

		float FlightTime = 2.5f;

		// 목표 위치에 도달하기 위한 발사 각도 계산
		float Angle = FMath::Atan2(HeightDifference + 0.5f * Gravity * FlightTime * FlightTime,
								   HorizontalDistance);

		UBFLMissileAttack::MissileAttack(Owner->GetAttackPoint(),
										 Target->GetActorLocation(),
										 Angle,
										 AttackDamage,
										 Owner,
										 GetWorld());
	}

	CallAttackLock();
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
	DamageLogic(Target, AttackDamage);
}

void UMonsterStatComponent::OnMissileHit(TArray<FOverlapResult> Targets)
{
	TSet<AActor*> CompleteTargets = {};

	if (Targets.IsEmpty())
	{
		return;
	}

	for (const FOverlapResult& Result : Targets)
	{

		AActor* Actor = Result.GetActor();
		if (!Actor)
		{
			continue;
		}

		if (CompleteTargets.Contains(Actor))
		{
			continue;
		}

		ABOCharacter* IsBOCharacter = Cast<ABOCharacter>(Actor);
		AMonsterCharacter* IsMonsterCharacter = Cast<AMonsterCharacter>(Actor);
		AAttackMissileActor* IsMissile = Cast<AAttackMissileActor>(Actor);

		if (IsBOCharacter || IsMonsterCharacter || IsMissile)
		{
			CompleteTargets.Add(Actor);
			DamageLogic(Actor, AttackDamage);
		}
	}
}

void UMonsterStatComponent::DamageLogic(AActor* Target, int32 Damage)
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

	UGameplayStatics::ApplyDamage(Target,
								  Damage,
								  AIController,
								  Owner,
								  UDamageType::StaticClass());
}

void UMonsterStatComponent::ApplyProtect(int32 GetDamage, AActor* DamageCauser)
{
	TakeDamage(FMath::Max(1, GetDamage - Protect), DamageCauser);
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
