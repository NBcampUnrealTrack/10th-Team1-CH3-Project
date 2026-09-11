#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "EquipmentHandlerComponent.generated.h"

class UEquippableItemInstance;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BEYONDOVERRIDE_API UEquipmentHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

  public:
	UEquipmentHandlerComponent();

	// 장비 등록
	virtual bool Assign(UEquippableItemInstance* EquippableItemInstance);
	// 장비 제거
	virtual bool Unassign();

	// 장비 장착
	virtual bool Equip();
	// 장비 해제
	virtual bool Unequip();

	// 장비 사용
	virtual bool Use();
};
