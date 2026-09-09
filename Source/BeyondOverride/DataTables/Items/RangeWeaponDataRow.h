#pragma once

#include "CoreMinimal.h"

#include "RangeWeaponDataRow.generated.h"

class ABulletProjectile;
class UWeaponAnimationDataAsset;

USTRUCT(BlueprintType)
struct BEYONDOVERRIDE_API FRangeWeaponDataRow : public FTableRowBase
{
	GENERATED_BODY()

  public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Damage;  // 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float FireRate;  // 발사 간격

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<ABulletProjectile> BulletClass;  // 투사체 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ProjectileSpeed;  // 투사체 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ProjectileGravityScale;  // 투사체 중력 스케일
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ProjectileRange;  // 사거리

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 MagazineSize;  // 탄창 크기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	float ReloadTime;  // 재장전 시간

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	TObjectPtr<UCurveFloat> RecoilPitchUpCurve;  // Pitch 반동
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	TObjectPtr<UCurveFloat> RecoilYawLeftCurve;  // Yaw 좌측 반동
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	TObjectPtr<UCurveFloat> RecoilYawRightCurve;  // Yaw 우측 반동

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spread")
	TObjectPtr<UCurveFloat> SpreadCurve;  // 탄 퍼짐

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UWeaponAnimationDataAsset> WeaponAnimationData;  // 애니메이션 데이터에셋
};
