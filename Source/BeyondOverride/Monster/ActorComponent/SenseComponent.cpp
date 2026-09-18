// 26/09/15 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/ActorComponent/SenseComponent.h"

// Add include
#include "DataTables/Monster/MonsterSense.h"
#include "Monster/DataAssets/MonsterDataAsset.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"

USenseComponent::USenseComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UMonsterDataAsset> DataAssetFinder(TEXT("/Game/Blueprints/Monster/DataAssets/DA_MonstersInfo.DA_MonstersInfo"));

	if (DataAssetFinder.Succeeded())
	{
		MonsterData = DataAssetFinder.Object;
	}
}

void USenseComponent::SetTarget(ABOCharacter* Target)
{
	MonsterTarget = Target;
}

ABOCharacter* USenseComponent::GetTarget() const
{
	return MonsterTarget;
}

void USenseComponent::SetTargetPoint(FVector Point)
{
	TargetPoint = Point;
}

FVector USenseComponent::GetTargetPoint() const
{
	return TargetPoint;
}

void USenseComponent::SetSpawnPoint(FVector Point)
{
	SpawnPoint = Point;
}

FVector USenseComponent::GetSpawnPoint() const
{
	return SpawnPoint;
}

float USenseComponent::GetHearSenseSize() const
{
	return HearSenseSize;
}

float USenseComponent::GetSightSenseSize() const
{
	return SightSenseSize;
}

float USenseComponent::GetLoseSightSize() const
{
	return LoseSightSize;
}

float USenseComponent::GetVisionAngleDegrees() const
{
	return LoseSightSize;
}

float USenseComponent::GetMemorize() const
{
	return Memorize;
}

void USenseComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USenseComponent::SenseSetup()
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

	FMonsterSense* MonsterSenseInfo = MonsterData->SenseTable->FindRow<FMonsterSense>(Monster->GetMonsterID(), TEXT("MonsterID Serching"));
	if (!MonsterSenseInfo)
	{
		Monster->SetMonsterID("Gunner");
		MonsterSenseInfo = MonsterData->StatTable->FindRow<FMonsterSense>(Monster->GetMonsterID(), TEXT("GunnerID Serching"));
	}

	HearSenseSize = MonsterSenseInfo->HearSenseSize;
	SightSenseSize = MonsterSenseInfo->SightSenseSize;
	LoseSightSize = MonsterSenseInfo->LoseSightSize;
	VisionAngleDegrees = MonsterSenseInfo->VisionAngleDegrees;
	Memorize = MonsterSenseInfo->Memorize;
}
