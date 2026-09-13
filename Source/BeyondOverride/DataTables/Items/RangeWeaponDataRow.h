#pragma once

#include "CoreMinimal.h"

#include "RangeWeaponDataRow.generated.h"

class ABulletProjectile;

USTRUCT(BlueprintType)
struct BEYONDOVERRIDE_API FRangeWeaponDataRow : public FTableRowBase
{
	GENERATED_BODY()

  public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	int32 Damage; // 데미지
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	float FireRate; // 발사 간격

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<ABulletProjectile> BulletClass; // 투사체 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float ProjectileSpeed; // 투사체 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float ProjectileGravityScale; // 투사체 중력 스케일
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float ProjectileRange; // 사거리

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
	int32 MagazineSize; // 탄창 크기
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
	float ReloadTime; // 재장전 시간

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recoil")
	TObjectPtr<UCurveFloat> RecoilPitchCurve; // Pitch 반동 (Up)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recoil")
	TObjectPtr<UCurveFloat> RecoilYawCurve; // Yaw 좌측 반동 (Right)

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spread")
	TObjectPtr<UCurveFloat> SpreadCurve; // 탄 퍼짐
};
