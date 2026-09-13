#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "MainScreenWidget.generated.h"

class UStatComponent;
class UProgressBar;

UCLASS()
class BEYONDOVERRIDE_API UMainScreenWidget : public UUserWidget
{
	GENERATED_BODY()

  protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ShieldBar;

  private:
	UPROPERTY()
	TObjectPtr<UStatComponent> StatComponent;

	UFUNCTION()
	void HandleHealthChanged(int32 Health, int32 MaxHealth);

	UFUNCTION()
	void HandleShieldChanged(int32 Shield, int32 MaxShield);
};
