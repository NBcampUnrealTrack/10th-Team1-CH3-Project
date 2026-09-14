#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "MainScreenWidget.generated.h"

class UStatComponent;
class UProgressBar;
class UPanelWidget;
class ABOCharacter;

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

  private:
	UPROPERTY()
	TObjectPtr<UStatComponent> StatComponent;

	UPROPERTY()
	TObjectPtr<ABOCharacter> OwningCharacter;

	UFUNCTION()
	void HandleHealthChanged(int32 Health, int32 MaxHealth);

	UFUNCTION()
	void HandleShieldChanged(int32 Shield, int32 MaxShield);

	void UpdateCompass();
};
