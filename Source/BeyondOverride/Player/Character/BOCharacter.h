#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"

#include "BOCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UEquipmentManagerComponent;
class UStatComponent;
class UPlayerInventoryComponent;
class UInventoryInteractionComponent;
class UInteractComponent;
class UEquipmentManagerComponent;

class UItemInstanceBase;
class UEquippableItemInstance;
class URangeWeaponInstance;

enum class EEquipmentSlot : uint8;

UCLASS()
class BEYONDOVERRIDE_API ABOCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UEquipmentManagerComponent* GetEquipmentComponent() const { return EquipmentManagerComponent; }
	UStatComponent* GetStatComponent() const { return StatComponent; }
	UPlayerInventoryComponent* GetPlayerInventoryComponent() const { return PlayerInventoryComponent; }
	UInteractComponent* GetInteractComponent() const { return InteractComponent; }
	UInventoryInteractionComponent* GetInventoryInteractionComponent() const { return InventoryInteractionComponent; }

	const bool GetIsAiming() const { return bIsAiming; }

public:
	ABOCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float WalkSpeed = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed = 600.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float CrouchSpeedMultiplier = 0.5f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SpeedMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Zoom")
	float DefaultFOV = 90.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Zoom")
	float AimFOV = 65.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Zoom")
	float ZoomSpeed = 20.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USkeletalMeshComponent* EquipmentSkeletalMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStatComponent* StatComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UPlayerInventoryComponent* PlayerInventoryComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UInventoryInteractionComponent* InventoryInteractionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UInteractComponent* InteractComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UEquipmentManagerComponent* EquipmentManagerComponent; // 장비 관리 컴포넌트

private:
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);
	UFUNCTION()
	void ToggleCrouch(const FInputActionValue& value);
	UFUNCTION()
	void Fire(const FInputActionValue& value);
	UFUNCTION()
	void Aim(const FInputActionValue& value);
	UFUNCTION()
	void Hip(const FInputActionValue& value);
	UFUNCTION()
	void Reload(const FInputActionValue& value);
	UFUNCTION()
	void InteractPress(const FInputActionValue& value);
	UFUNCTION()
	void InteractRelease(const FInputActionValue& value);
	UFUNCTION()
	void Inventory(const FInputActionValue& value);
	UFUNCTION()
	void Escape(const FInputActionValue& value);

	UFUNCTION()
	void EquipSlot1(const FInputActionValue& value);
	UFUNCTION()
	void EquipSlot2(const FInputActionValue& value);
	UFUNCTION()
	void EquipSlot3(const FInputActionValue& value);
	UFUNCTION()
	void EquipSlot4(const FInputActionValue& value);
	UFUNCTION()
	void EquipSlot5(const FInputActionValue& value);
	UFUNCTION()
	void Unarm(const FInputActionValue& value);
	UFUNCTION()
	void DropEquipment(const FInputActionValue& value);

	void ChangeMoveSpeed();

	bool bIsSprint = false;
	bool bIsAiming = false;

	UFUNCTION(Exec)
	void AddTestItem(FName ItemID, int32 Count = 1);

public:
	// 장비 슬롯에 아이템 등록 및 해제
	void OnEquipmentSlotChanged(EEquipmentSlot Slot, UItemInstanceBase* ItemInstanceBase);

	// EquipmentManagerComponent의 델리게이트 바인딩
	void BindingEquipmentManagerComponentDelegates();
	// EquipmentManagerComponent - 장비 애니메이션 설정
	void OnEquipmentChanged(UEquippableItemInstance* EquippableItemInstance);
	// EquipmentManagerComponent - Range Weapon 델리게이트 연결 이벤트
	bool OnCanReload(URangeWeaponInstance* RangeWeaponInstance) const;
	int32 OnRequestReloadAmmo(URangeWeaponInstance* RangeWeaponInstance);
};
