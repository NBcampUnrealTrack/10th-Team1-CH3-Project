
#pragma once

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"

#include "EquipmentAnimationDataAsset.generated.h"

UCLASS()
class UEquipmentAnimationDataAsset : public UDataAsset
{
	GENERATED_BODY()

  public:
	UPROPERTY(EditDefaultsOnly, Category = "Character|Hip")
	TObjectPtr<UAnimSequence> HipIdle;  // 캐릭터 비조준 Idle
	UPROPERTY(EditDefaultsOnly, Category = "Character|Hip")
	TObjectPtr<UBlendSpace> HipLocomotion;  // 캐릭터 비조준 Locomotion

	UPROPERTY(EditDefaultsOnly, Category = "Character|Aim")
	TObjectPtr<UAnimSequence> AimIdle;  // 캐릭터 조준 Idle
	UPROPERTY(EditDefaultsOnly, Category = "Character|Aim")
	TObjectPtr<UBlendSpace> AimLocomotion;  // 캐릭터 조준 Locomotion

	UPROPERTY(EditDefaultsOnly, Category = "Character|Airborne")
	TObjectPtr<UAnimSequence> Jump;  // 캐릭터 점프
	UPROPERTY(EditDefaultsOnly, Category = "Character|Airborne")
	TObjectPtr<UAnimSequence> FallingLoop;  // 캐릭터 낙하 (반복)
	UPROPERTY(EditDefaultsOnly, Category = "Character|Airborne")
	TObjectPtr<UAnimSequence> Land;  // 캐릭터 착지

	UPROPERTY(EditDefaultsOnly, Category = "Character|Action")
	TObjectPtr<UAnimMontage> Equip;  // 캐릭터 무기 Equip 애니메이션
	UPROPERTY(EditDefaultsOnly, Category = "Character|Action")
	TObjectPtr<UAnimMontage> Fire;  // 캐릭터 무기 Fire 애니메이션
	UPROPERTY(EditDefaultsOnly, Category = "Character|Action")
	TObjectPtr<UAnimMontage> Reload;  // 캐릭터 무기 Reload 애니메이션

	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Action")
	TObjectPtr<UAnimMontage> WeaponFire;  // 총기 Fire 애니메이션
	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Action")
	TObjectPtr<UAnimMontage> WeaponReload;  // 총기 Reload 애니메이션
};
