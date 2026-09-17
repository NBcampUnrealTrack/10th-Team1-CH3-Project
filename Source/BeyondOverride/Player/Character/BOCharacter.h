#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"

#include "BOCharacter.generated.h"

class UCameraShakeBase;
class USpringArmComponent;
class UCameraComponent;
class UEquipmentManagerComponent;
class UStatComponent;
class UPlayerInventoryComponent;
class UInventoryInteractionComponent;
class UInteractComponent;
class UNearbyItemComponent;

class UItemInstanceBase;
class UEquippableItemInstance;

class UAnimMontage;

struct FUtilityItemDataRow;

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
	UNearbyItemComponent* GetNearbyItemComponent() const { return NearbyItemComponent; }

	bool GetIsAiming() const { return bIsAiming; }

	UFUNCTION(BlueprintCallable)
	void SetMovementEnabled(bool bEnabled);

	UFUNCTION(BlueprintPure)
	bool IsMovementEnabled() const { return bMovementEnabled; }

	UFUNCTION(BlueprintPure)
	bool IsOverweight() const { return bIsOverweight; }

public:
	ABOCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> DeathMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> RollMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera|Shake", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShakeBase> DamageCameraShakeClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float WalkSpeed = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed = 600.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float RollSpeed = 600.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float CrouchSpeedMultiplier = 0.5f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SpeedMultiplier = 1.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement|Weight")
	float WeightSpeedMultiplier = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Weight")
	float MinWeightSpeedMultiplier = 0.1f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement|Weight")
	bool bIsOverweight = false;

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
	UNearbyItemComponent* NearbyItemComponent;
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
	void StartFire(const FInputActionValue& value);
	UFUNCTION()
	void CompleteFire(const FInputActionValue& value);

	UFUNCTION()
	void Aim(const FInputActionValue& value);
	UFUNCTION()
	void Hip(const FInputActionValue& value);
	UFUNCTION()
	void Reload(const FInputActionValue& value);

	UFUNCTION()
	void Roll(const FInputActionValue& Value);
	UFUNCTION()
	void StartRoll();
	UFUNCTION()
	void StopRoll();

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

	bool bIsRolling = false;
	bool bIsSprint = false;
	bool bIsAiming = false;
	bool bMovementEnabled = true;
	bool bGameplayInputEnabled = true;

	UFUNCTION(Exec)
	void AddTestItem(FName ItemID, int32 Count = 1);
	UFUNCTION()
	void OnMenuOpenStateChanged(bool bAnyMenuOpen);
	void UpdateMovementEnabled();

	void HandleDamaged();

	// 사망 관련
	UFUNCTION()
	void HandleDeath(AActor* DamageCauser);
	void FinishPlayerDeath();

	TWeakObjectPtr<AActor> DeathDamageCauser;
	bool bDeathSequenceFinished = false;

public:
	// 장비 슬롯에 아이템 등록 및 해제
	UFUNCTION()
	void OnEquipmentItemChanged(EEquipmentSlot Slot, UItemInstanceBase* ItemInstanceBase);

	void TryEquipSlot(EEquipmentSlot Slot);

	UFUNCTION()
	void OnWeightChanged(float CurCarryWeight, float MaxCarryWeight);

private:
	// EquipmentManagerComponent의 델리게이트 바인딩
	void BindingEquipmentManagerComponentDelegates();

	// EquipmentManagerComponent::OnActiveSlotChangedDelegate 바인딩 - 활성화 슬롯 변경 시 호출
	void OnActiveSlotChanged(EEquipmentSlot Slot, UEquippableItemInstance* EquippableItemInstance);

	// EquipmentManagerComponent::OnFireExecutedDelegate 바인딩 - 사격 실행 시 호출, 사격 애니메이션 재생
	void OnFireExecuted() const;
	// EquipmentManagerComponent::CanReloadDelegate 바인딩 - 재장전 시도 시 호출, 가능 여부 반환
	bool CanReload(const FName& AmmoItemID) const;
	// EquipmentManagerComponent::RequestReloadAmmoDelegate 바인딩 - 재장전 완료 시 호출, 보충할 개수 반환
	int32 RequestReloadAmmo(const FName& AmmoItemID, const int32 RequestedAmmoCount);

	// EquipmentManagerComponent::OnEquipmentCountUpdatedDelegate 바인딩 - Throwable & Utility 아이템 사용 후 호출, 해당 슬롯 아이템의 스택 개수 변경됨을 알림
	void OnEquipmentCountUpdated(EEquipmentSlot Slot, UEquippableItemInstance* EquippableItemInstance);

	// EquipmentManagerComponent::CanUseUtilityItemDelegate 바인딩 - 유틸리티 아이템 사용 전 호출, 해당 아이템 사용 가능한지 여부 반환
	bool CanUseUtilityItem(const FUtilityItemDataRow* UtilityItemData) const;
	// EquipmentManagerComponent::OnEffectAppliedDelegate 바인딩 - 유틸리티 아이템 사용 후 호출, 해당 아이템 효과 적용
	void OnEffectApplied(const FUtilityItemDataRow* UtilityItemData);

private:
	FTimerHandle DeathTimerHandle;

	FVector2D MoveInput = FVector2D::ZeroVector;
	FVector RollDirection = FVector::ZeroVector;

	FName GetRollSectionName() const;

	FVector GetRollDirection() const;
	void OnRollMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	void OnReloadMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	bool CanUseGameplayInput() const;
	void StopGameplayActions();
};
