#pragma once

#include "CoreMinimal.h"

#include "DataTables/Items/ItemDataRow.h"
#include "Engine/DataAsset.h"

#include "ItemDataAsset.generated.h"

class UItemInstanceBase;
class AItemPickupBase;

UENUM(BlueprintType)
enum class EItemID : uint8
{
	None,
};

UCLASS()
class UItemDataAsset : public UDataAsset
{
	GENERATED_BODY()

  public:
	UPROPERTY(EditDefaultsOnly, Category = "Class")
	TSubclassOf<UItemInstanceBase> ItemInstanceClass;
	UPROPERTY(EditDefaultsOnly, Category = "Class")
	TSubclassOf<AItemPickupBase> ItemPickupClass;

	UPROPERTY(EditDefaultsOnly, Category = "Identity")
	EItemID ItemID;
	UPROPERTY(EditDefaultsOnly, Category = "Identity")
	EItemType ItemType;
	UPROPERTY(EditDefaultsOnly, Category = "Identity")
	EItemRarity ItemRarity;

	UPROPERTY(EditDefaultsOnly, Category = "Info")
	FName ItemName;
	UPROPERTY(EditDefaultsOnly, Category = "Info")
	FText Description;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	float Weight;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 MaxStackCount;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TObjectPtr<UTexture2D> ItemIcon;
};
