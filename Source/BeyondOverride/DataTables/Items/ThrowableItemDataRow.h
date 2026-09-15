
#pragma once

#include "CoreMinimal.h"

#include "ThrowableItemDataRow.generated.h"

class AThrowableProjectile;

USTRUCT(BlueprintType)
struct BEYONDOVERRIDE_API FThrowableItemDataRow : public FTableRowBase
{
	GENERATED_BODY()

  public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	int32 Damage; // 데미지
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	float Radius; // 범위
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	float ActivationDelay; // 투척 후 활성화까지 시간

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<AThrowableProjectile> ThrowableClass; // 투사체 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float ThrowSpeed; // 던지는 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float ThrowGravityScale; // 중력 스케일
};
