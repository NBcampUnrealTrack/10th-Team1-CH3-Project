// 26/09/15 Copyright Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// UHT Header
#include "MonsterInfo.generated.h"

USTRUCT(BlueprintType)
struct FMonsterInfo : public FTableRowBase
{

	GENERATED_BODY()

  public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MonsterID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MonsterAttackSocket;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> MonsterSkeletal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> MonsterAnimInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UParticleSystem> MonsterAttackEffect;
};
