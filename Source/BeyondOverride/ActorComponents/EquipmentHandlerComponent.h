#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "EquipmentHandlerComponent.generated.h"

class USkeletalMeshComponent;
class UEquippableItemInstance;
class UAnimMontage;

struct FItemDataRow;
struct FEquippableItemDataRow;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BEYONDOVERRIDE_API UEquipmentHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// 캐릭터에 부착 및 장비 메시를 설정할 메시 컴포넌트
	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> EquipMeshComponent;

	// 현재 등록된 장비
	UPROPERTY()
	TObjectPtr<UEquippableItemInstance> EquippableItemInstance;

	// 현재 장비의 고유 데이터
	const FItemDataRow* ItemData;
	const FEquippableItemDataRow* EquippableItemData;

	// 장착 몽타주가 재생 중인지
	bool bIsEquipping = false;

	// 현재 재생 중인 Equip 몽타주
	UPROPERTY()
	TObjectPtr<UAnimMontage> PlayingEquipMontage;

	// 장착 딜레이 타이머 핸들
	FTimerHandle EquipTimerHandle;

public:
	UEquipmentHandlerComponent();

protected:
	virtual void OnRegister() override;

public:
	// 현재 등록된 장비가 있는지 여부
	bool HasEquipment() const;

	// 등록된 장비 반환
	UEquippableItemInstance* GetEquippableItemInstance() const;

	// 장비 등록
	virtual bool Assign(UEquippableItemInstance* InEquippableItemInstance);
	// 장비 제거
	virtual UEquippableItemInstance* Unassign();

	// 장비 장착
	virtual bool Equip();
	// 장비 해제
	virtual bool Unequip();

	// 장비 사용
	virtual bool Use();

	// 사용 시작
	virtual void StartAction();
	// 사용 종료
	virtual void EndAction();

protected:
	// 장비 등록 가능 여부
	virtual bool CanAssign(const UEquippableItemInstance* InEquippableItemInstance) const;
	// 장비 제거 가능 여부
	virtual bool CanUnassign() const;
	// 장비 장착 가능 여부
	virtual bool CanEquip() const;
	// 장비 해제 가능 여부
	virtual bool CanUnequip() const;
	// 장비 사용 가능 여부
	virtual bool CanUse() const;

	// 소켓에 메시 부착
	void AttachToSocket(const FName& SocketName, bool bHideIfNoSocket = false);

protected:
	// 장비 장착 시작 시 호출
	virtual void OnEquipStarted();
	// 장비 장착 완료 시 호출
	virtual void OnEquipCompleted();
	// 장비 장착 중단 시 호출
	virtual void OnEquipInterrupted();

	// 장비 장착 중인지 여부
	bool IsEquipping() const;

private:
	void HandleEquipMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
