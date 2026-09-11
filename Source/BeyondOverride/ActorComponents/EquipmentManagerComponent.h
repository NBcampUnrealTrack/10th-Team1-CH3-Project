#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "EquipmentManagerComponent.generated.h"

enum class EEquipmentSlot : uint8;

class UEquipmentHandlerComponent;
class UItemInstanceBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BEYONDOVERRIDE_API UEquipmentManagerComponent : public UActorComponent
{
	GENERATED_BODY()

  protected:
	// 활성화된 장비 슬롯
	EEquipmentSlot ActiveSlot;
	// 각 장비 슬롯 별 컴포넌트
	TMap<EEquipmentSlot, UEquipmentHandlerComponent*> EquipmentHandlerComponents;

  public:
	UEquipmentManagerComponent();

	// 슬롯 전환
	void Equip(EEquipmentSlot Slot);
	// 비무장 전환
	void Unequip(); // 미구현

	// 현재 장비 사용
	void Use();

	// 슬롯에 장비 등록
	void Assign(EEquipmentSlot Slot, UItemInstanceBase* ItemInstanceBase);
	// 슬롯에서 장비 해제
	void Unassign(EEquipmentSlot Slot);
};
