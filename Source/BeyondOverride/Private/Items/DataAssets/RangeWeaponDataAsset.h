#pragma once

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"

#include "RangeWeaponDataAsset.generated.h"

class ABulletProjectile;
class UWeaponAnimationDataAsset;

UCLASS()
class URangeWeaponDataAsset : public UDataAsset
{
	GENERATED_BODY()

  public:
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	int32 Damage;  // 데미지
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float FireRate;  // 발사 간격

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<ABulletProjectile> BulletClass;  // 투사체 클래스
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float ProjectileSpeed;  // 투사체 속도
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float ProjectileGravityScale;  // 투사체 중력 스케일
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float ProjectileRange;  // 사거리

	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	int32 MagazineSize;  // 탄창 크기
	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	float ReloadTime;  // 재장전 시간

	UPROPERTY(EditDefaultsOnly, Category = "Recoil")
	TObjectPtr<UCurveFloat> RecoilPitchUpCurve;  // Pitch 반동
	UPROPERTY(EditDefaultsOnly, Category = "Recoil")
	TObjectPtr<UCurveFloat> RecoilYawLeftCurve;  // Yaw 좌측 반동
	UPROPERTY(EditDefaultsOnly, Category = "Recoil")
	TObjectPtr<UCurveFloat> RecoilYawRightCurve;  // Yaw 우측 반동

	UPROPERTY(EditDefaultsOnly, Category = "Spread")
	TObjectPtr<UCurveFloat> SpreadCurve;  // 탄 퍼짐

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UWeaponAnimationDataAsset> WeaponAnimationData;  // 애니메이션 데이터에셋
};
