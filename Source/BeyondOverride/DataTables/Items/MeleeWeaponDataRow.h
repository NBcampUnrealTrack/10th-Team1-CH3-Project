#pragma once

#include "CoreMinimal.h"

#include "MeleeWeaponDataRow.generated.h"

USTRUCT(BlueprintType)
struct BEYONDOVERRIDE_API FMeleeWeaponDataRow : public FTableRowBase
{
	GENERATED_BODY()

  public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	int32 Damage; // 데미지
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	float AttackInterval; // 공격 간격
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	float AttackRadius; // 공격 범위
};
