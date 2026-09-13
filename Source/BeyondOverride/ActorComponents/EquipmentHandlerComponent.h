#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "EquipmentHandlerComponent.generated.h"

class USkeletalMeshComponent;
class UEquippableItemInstance;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BEYONDOVERRIDE_API UEquipmentHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

  protected:
	TObjectPtr<USkeletalMeshComponent> EquipMeshComponent;

  public:
	UEquipmentHandlerComponent();

  protected:
	virtual void OnRegister() override;

  public:
	// 등록된 장비 반환
	virtual UEquippableItemInstance* GetEquippableItemInstance() const;

	// 장비 등록
	virtual bool Assign(UEquippableItemInstance* EquippableItemInstance);
	// 장비 제거
	virtual UEquippableItemInstance* Unassign();

	// 장비 장착
	virtual bool Equip();
	// 장비 해제
	virtual bool Unequip();

	// 장비 사용
	virtual bool Use();

	// 장비 해제 가능 여부
	virtual bool CanUnequip();

  protected:
	// 소켓에 메시 부착
	void AttachToSocket(FName SocketName);
};
