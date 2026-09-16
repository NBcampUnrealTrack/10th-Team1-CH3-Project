#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "DamageFlashWidget.generated.h"

class UStatComponent;
class ABOCharacter;

UCLASS()
class BEYONDOVERRIDE_API UDamageFlashWidget : public UUserWidget
{
	GENERATED_BODY()

  protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Damage Flash", meta = (DisplayName = "On Shield Hit"))
	void PlayShieldFlash();

	UFUNCTION(BlueprintImplementableEvent, Category = "Damage Flash", meta = (DisplayName = "On Health Hit"))
	void PlayHealthFlash();

  private:
	UFUNCTION()
	void HandleShieldChanged(int32 CurShield, int32 MaxShield);
	UFUNCTION()
	void HandleHealthChanged(int32 CurHealth, int32 MaxHealth);

	UPROPERTY()
	TObjectPtr<UStatComponent> StatComponent;

	int32 LastShield = 0;
	int32 LastHealth = 0;
};
