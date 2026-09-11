// TODO:
//	- 비무장 구현
//		- Unarmed(MeleeWeaponInstance) 생성 및 저장
//		- 비무장 시 UnarmedHandler(MeleeWeaponHandlerComponent)에 Unarmed 장비 등록하기
//		- 비무장 전환(Unequip) 또는 빈 슬롯 전환(Equip) 시 UnarmedHandler을 장착하기
//	- 결합도 낮추기
//		- UBOAnimInstance를 직접 불러오지 않고, 델리게이트 등을 통해 전달하기

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
