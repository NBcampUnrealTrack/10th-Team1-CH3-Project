#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "MainScreenWidget.generated.h"

class UStatComponent;
class UProgressBar;
class UPanelWidget;
class ABOCharacter;
class UPlayerInventoryComponent;
class UItemInstanceBase;
class UEquipmentManagerComponent;
enum class EEquipmentSlot : uint8;

UCLASS()
class BEYONDOVERRIDE_API UMainScreenWidget : public UUserWidget
{
	GENERATED_BODY()

  protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ShieldBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> CompassTick;

	// 실제로 보여지는 나침반 창의 폭. SizeBox의 Width Override와 반드시 같은 값으로 맞춰야 함.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compass")
	float CompassViewWidth = 600.0f;

	// 나침반 정렬이 살짝 어긋날 때 눈으로 보면서 미세조정하는 보정값 (도 단위)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compass")
	float CompassYawOffset = 7.5f;

	// 눈금 하나(15도)당 실제 픽셀 폭
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compass")
	float TickUnitWidth = 40.0f;

	// 눈금 하나가 몇 도 간격인지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compass")
	float DegreesPerTick = 15.0f;

  protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidget> LineTop;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidget> LineBottom;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidget> LineLeft;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidget> LineRight;

	// 탄 퍼짐이 0도일 때 중심에서 각 조각까지의 기본 거리 (픽셀)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crosshair")
	float CrosshairBaseOffset = 0.0f;

	// 탄 퍼짐 1도당 추가로 벌어지는 거리 (픽셀)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crosshair")
	float CrosshairPixelsPerDegree = 40.0f;

	// 벌어질 수 있는 최대 거리 (픽셀)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crosshair")
	float CrosshairMaxOffset = 120.0f;

	// 목표 거리를 따라가는 속도 (클수록 빠르게 반응)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crosshair")
	float CrosshairInterpSpeed = 25.0f;

  private:
	UPROPERTY()
	TObjectPtr<UStatComponent> StatComponent;

	UPROPERTY()
	TObjectPtr<ABOCharacter> OwningCharacter;

	UPROPERTY()
	TObjectPtr<UPlayerInventoryComponent> InventoryComponent;

		UPROPERTY()
	TObjectPtr<UEquipmentManagerComponent> EquipmentManager;

	float TargetSpreadDegree = 0.0f;

	float CurrentCrosshairOffset = 0.0f;

	void HandleSpreadDegreeUpdated(float SpreadDegree);

	void UpdateCrosshair(float DeltaTime);

	UFUNCTION()
	void HandleHealthChanged(int32 Health, int32 MaxHealth);

	UFUNCTION()
	void HandleShieldChanged(int32 Shield, int32 MaxShield);

	UFUNCTION()
	void HandleEquipmentSlotChanged(EEquipmentSlot ChangedSlot, UItemInstanceBase* ItemInstanceBase);

	void UpdateCompass();
};
