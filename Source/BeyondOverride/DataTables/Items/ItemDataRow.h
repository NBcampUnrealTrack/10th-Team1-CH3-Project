#pragma once

#include "CoreMinimal.h"

#include "ItemDataRow.generated.h"

class UItemInstanceBase;
class AItemPickupBase;

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

USTRUCT(BlueprintType)
struct BEYONDOVERRIDE_API FItemDataRow : public FTableRowBase
{
	GENERATED_BODY()

  public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	FText DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Category")
	EItemType ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Category")
	EItemRarity ItemRarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
	TSubclassOf<UItemInstanceBase> ItemInstanceClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
	TSubclassOf<AItemPickupBase> ItemPickupClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 MaxStackCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<UTexture2D> ItemIcon;
};
