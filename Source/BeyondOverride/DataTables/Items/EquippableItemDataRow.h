#pragma once

#include "CoreMinimal.h"

#include "EquippableItemDataRow.generated.h"

class UEquipmentAnimationDataAsset;

USTRUCT(BlueprintType)
struct BEYONDOVERRIDE_API FEquippableItemDataRow : public FTableRowBase
{
	GENERATED_BODY()

  public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Equip")
	TObjectPtr<USkeletalMesh> EquipMesh; // 장착할 메시
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Equip")
	FName EquipSocketName; // 장착 소켓 이름
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Equip")
	FName HolsterSocketName; // 사용하지 않을 때 소켓 이름

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UEquipmentAnimationDataAsset> EquipmentAnimationData; // 장비 애니메이션 데이터에셋
};
