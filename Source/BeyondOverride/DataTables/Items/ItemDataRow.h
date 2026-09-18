#pragma once

#include "CoreMinimal.h"

#include "ItemDataRow.generated.h"

class UItemInstanceBase;
class AItemPickupBase;

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None,
	Misc,          // 기타
	RangeWeapon,   // 원거리 무기
	MeleeWeapon,   // 근접 무기
	ThrowableItem, // 투척 아이템
	EffectItem,    // 효과 아이템
	BagItem,
	ShieldItem
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

USTRUCT(BlueprintType)
struct BEYONDOVERRIDE_API FItemDataRow : public FTableRowBase
{
	GENERATED_BODY()

  public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Info")
	FText DisplayName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Info")
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Category")
	EItemType ItemType = EItemType::None;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Category")
	EItemRarity ItemRarity = EItemRarity::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Class")
	TSubclassOf<UItemInstanceBase> ItemInstanceClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Class")
	TSubclassOf<AItemPickupBase> ItemPickupClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Asset")
	TObjectPtr<UStaticMesh> ItemPickupMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	int32 MaxStackCount = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	float Weight = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TObjectPtr<UTexture2D> ItemIcon;
};
