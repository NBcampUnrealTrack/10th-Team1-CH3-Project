#pragma once

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"

#include "EquippableItemDataAsset.generated.h"

UCLASS()
class UEquippableItemDataAsset : public UDataAsset
{
	GENERATED_BODY()

  public:
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	TObjectPtr<USkeletalMesh> EquipMesh;
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	FName EquipSocketName;
};
