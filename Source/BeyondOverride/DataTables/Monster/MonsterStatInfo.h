// 26/09/15 Copyright Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Add include
#include "Monster/ActorComponent/MonsterStatComponent.h"

// UHT Header
#include "MonsterStatInfo.generated.h"

USTRUCT(BlueprintType)
struct FMonsterStatInfo : public FTableRowBase
{
	GENERATED_BODY()

  public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MonsterID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AttackDamage = 15;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RapidCount = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RapidDelay = 0.05f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackDelay = 7.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange = 600.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BulletSpeed = 2500.0f;

	// Another Info
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Protect = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Intelligence = FMath::RandRange(0, 5);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeed = 600;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SprintSpeed = 1.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMonsterType MonsterType = EMonsterType::Range;
};
