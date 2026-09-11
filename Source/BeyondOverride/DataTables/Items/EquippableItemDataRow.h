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
	TObjectPtr<USkeletalMesh> EquipMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Equip")
	FName EquipSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UEquipmentAnimationDataAsset> EquipmentAnimationData; // 장비 애니메이션 데이터에셋
};
