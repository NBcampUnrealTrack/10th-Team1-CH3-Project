
#pragma once

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"

#include "EquipmentAnimationDataAsset.generated.h"

UCLASS()
class UEquipmentAnimationDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	// Character Locomotion
	UPROPERTY(EditDefaultsOnly, Category = "Character|Locomotion")
	TObjectPtr<UBlendSpace> LocomotionHip; // 캐릭터 Hip Locomotion
	UPROPERTY(EditDefaultsOnly, Category = "Character|Locomotion")
	TObjectPtr<UBlendSpace> LocomotionAim; // 캐릭터 Aim Locomotion

	// Character Airborne
	UPROPERTY(EditDefaultsOnly, Category = "Character|Airborne")
	TObjectPtr<UAnimSequence> Jump; // 캐릭터 점프
	UPROPERTY(EditDefaultsOnly, Category = "Character|Airborne")
	TObjectPtr<UAnimSequence> FallingLoop; // 캐릭터 낙하 (반복)
	UPROPERTY(EditDefaultsOnly, Category = "Character|Airborne")
	TObjectPtr<UAnimSequence> Land; // 캐릭터 착지

	// Character Action
	UPROPERTY(EditDefaultsOnly, Category = "Character|Action")
	TObjectPtr<UAnimMontage> Equip; // 캐릭터 무기 Equip 애니메이션
	UPROPERTY(EditDefaultsOnly, Category = "Character|Action")
	TObjectPtr<UAnimMontage> FireHip; // 캐릭터 무기 Hip Fire 애니메이션
	UPROPERTY(EditDefaultsOnly, Category = "Character|Action")
	TObjectPtr<UAnimMontage> FireAim; // 캐릭터 무기 Aim Fire 애니메이션
	UPROPERTY(EditDefaultsOnly, Category = "Character|Action")
	TObjectPtr<UAnimMontage> ReloadHip; // 캐릭터 무기 Hip Reload 애니메이션
	UPROPERTY(EditDefaultsOnly, Category = "Character|Action")
	TObjectPtr<UAnimMontage> ReloadAim; // 캐릭터 무기 Aim Reload 애니메이션

	// Weapon Action
	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Action")
	TObjectPtr<UAnimMontage> WeaponFire; // 총기 Fire 애니메이션
	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Action")
	TObjectPtr<UAnimMontage> WeaponReloadHip; // 총기 Hip Reload 애니메이션
	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Action")
	TObjectPtr<UAnimMontage> WeaponReloadAim; // 총기 Aim Reload 애니메이션
};
