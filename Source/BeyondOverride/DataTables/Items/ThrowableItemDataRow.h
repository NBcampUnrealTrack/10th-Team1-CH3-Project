
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
	int32 Damage = 0; // 데미지
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	float Radius = 0.f; // 범위
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	float ThrowDuration = 0.f; // 투척에 걸리는 시간
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	float ActivationDelay = 0.f; // 투척 후 활성화까지 시간

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<AThrowableProjectile> ThrowableClass; // 투사체 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float ThrowSpeed = 0.f; // 던지는 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float ThrowGravityScale = 1.f; // 중력 스케일
};
