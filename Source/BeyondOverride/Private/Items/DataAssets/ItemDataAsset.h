#pragma once

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"

#include "ItemDataAsset.generated.h"

class UItemInstanceBase;
class AItemPickupBase;

UENUM(BlueprintType)
enum class EItemID : uint8
{
	None,
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None,
	Misc,           // 기타
	RangeWeapon,    // 원거리 무기
	MeleeWeapon,    // 근접 무기
	ThrowableItem,  // 투척 아이템
	EffectItem,     // 효과 아이템
};

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	None,
	Common,
	Uncommon,
	Rare,
	Epic,
	Legendary,
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
