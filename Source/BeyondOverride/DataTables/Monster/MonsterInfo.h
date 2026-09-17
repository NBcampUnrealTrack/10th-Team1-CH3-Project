// 26/09/15 Copyright Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// UHT Header
#include "MonsterInfo.generated.h"

class AMonsterCharacter;

USTRUCT(BlueprintType)
struct FMonsterInfo : public FTableRowBase
{

	GENERATED_BODY()

  public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MonsterID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CapsuleRadius = 34.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CapsuleHalfHeight = 88.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SkeletalScale = FVector(1.0f, 1.0f, 1.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator SkeletalRotation = FRotator(0.0f, -90.0f, 0.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SkeletalLocation = FVector(0.0f, 0.0f, -90.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MonsterAttackSocket;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> MonsterSkeletal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> MonsterAnimInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UParticleSystem> MonsterAttackEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UParticleSystem> MonsterImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MonsterName;
};
