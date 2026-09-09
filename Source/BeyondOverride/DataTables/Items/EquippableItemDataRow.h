#pragma once

#include "CoreMinimal.h"

#include "EquippableItemDataRow.generated.h"

USTRUCT(BlueprintType)
struct BEYONDOVERRIDE_API FEquippableItemDataRow : public FTableRowBase
{
	GENERATED_BODY()

  public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Equip")
	TObjectPtr<USkeletalMesh> EquipMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Equip")
	FName EquipSocketName;
};
