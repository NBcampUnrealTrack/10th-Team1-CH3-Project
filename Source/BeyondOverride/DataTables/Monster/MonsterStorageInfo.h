// 26/09/28 Copyright Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// UHT Header
#include "MonsterStorageInfo.generated.h"

USTRUCT(BlueprintType)
struct FMonsterStorageInfo : public FTableRowBase
{
	GENERATED_BODY()

  public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SkeletalScale = FVector(10.0f, 10.0f, 10.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator SkeletalRotation = FRotator(0.0f, 0.0f, 0.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SkeletalLocation = FVector(0.0f, 0.0f, 0.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> StorageSkeletal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> StorageAnimInstance;
};
