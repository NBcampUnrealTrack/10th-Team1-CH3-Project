#pragma once

#include "CoreMinimal.h"

#include "Enums/FireMode.h"

#include "RangeWeaponDataRow.generated.h"

class ABulletBase;

USTRUCT(BlueprintType)
struct BEYONDOVERRIDE_API FRangeWeaponDataRow : public FTableRowBase
{
	GENERATED_BODY()

  public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	int32 Damage = 0; // 데미지
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	float FireRate = 0.f; // 발사 간격
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	EFireMode FireMode = EFireMode::None; // 사격 모드

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<ABulletBase> BulletClass; // 투사체 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float ProjectileSpeed = 0.f; // 투사체 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float ProjectileGravityScale = 1.f; // 투사체 중력 스케일
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float ProjectileRange = 0.f; // 사거리
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	int32 ProjectilesPerShot = 1; // 사격 당 투사체 개수

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
	FName AmmoItemID; // 탄약 아이템 ID
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
	int32 MagazineSize = 1; // 탄창 크기
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
	float ReloadTime = 0.f; // 재장전 시간

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recoil")
	TObjectPtr<UCurveFloat> RecoilPitchCurve; // Pitch 반동 (Up)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recoil")
	TObjectPtr<UCurveFloat> RecoilYawCurve; // Yaw 좌측 반동 (Right)

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spread")
	TObjectPtr<UCurveFloat> SpreadCurve; // 탄 퍼짐 (비조준 기준)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spread")
	float AimSpreadMultiplier = 1.f; // 조준 시 탄 퍼짐 배율

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Icon")
	TObjectPtr<UTexture2D> ItemIconLong;
};
