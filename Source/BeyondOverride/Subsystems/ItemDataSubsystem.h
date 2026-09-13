#pragma once

#include "CoreMinimal.h"

#include "Subsystems/GameInstanceSubsystem.h"

#include "ItemDataSubsystem.generated.h"

class UItemDataRegistry;

struct FItemDataRow;
struct FEquippableItemDataRow;
struct FRangeWeaponDataRow;
struct FMeleeWeaponDataRow;

UCLASS()
class BEYONDOVERRIDE_API UItemDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

  protected:
	TObjectPtr<UItemDataRegistry> ItemDataRegistry;

  public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

  public:
	// ItemID를 통해 각 데이터테이블 행 반환
	const FItemDataRow* GetItemData(const FName ItemID) const;
	const FEquippableItemDataRow* GetEquippableItemData(const FName ItemID) const;
	const FRangeWeaponDataRow* GetRangeWeaponData(const FName ItemID) const;
	const FMeleeWeaponDataRow* GetMeleeWeaponData(const FName ItemID) const;
};
