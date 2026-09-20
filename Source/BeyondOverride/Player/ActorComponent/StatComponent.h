#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, int32, CurHealth, int32, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShieldChanged, int32, CurShield, int32, MaxShield);
DECLARE_MULTICAST_DELEGATE(FOnDamaged);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BEYONDOVERRIDE_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void TakeDamage(int32 DamageAmount, AActor* DamageCauser);
	void Heal(int32 HealAmount);

	void SetCurHealth(int32 NewCurHealth);
	void SetMaxHealth(int32 NewMaxHealth);
	void SetCurShield(int32 NewCurShield);
	void SetMaxShield(int32 NewMaxShield);

	int32 GetCurHealth() const { return CurHealth; }
	int32 GetMaxHealth() const { return MaxHealth; }
	int32 GetCurShield() const { return CurShield; }
	int32 GetMaxShield() const { return MaxShield; }
	bool GetIsDead() const { return bIsDead; }

	void ApplyShield(int32 NewCurrentShield, int32 NewMaxShield, float NewRegenDelay, float NewRegenInterval, int32 NewRegenAmount);
	void RemoveShield();

public:
	FOnHealthChanged OnHealthChanged; // 체력이 변경됐을 때 실행할 델리게이트
	FOnShieldChanged OnShieldChanged; // 쉴드량이 변경됐을 때 실행할 델리게이트
	FOnDamaged OnDamaged; // 피해를 받았을 때 실행할 델리게이트
	FOnDeath OnDeath; // 사망했을 때 실행할 델리게이트

public:
	UStatComponent();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	int32 MaxHealth = 100;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	int32 CurHealth = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	int32 MaxShield = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	int32 CurShield = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	bool bIsDead = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	float ShieldDelayTime = 10.0f; // 피격 이후 첫 쉴드가 차기까지의 대기시간 관리
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	float ShieldRegenTime = 0.5f; // 쉴드가 차는 주기 관리
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	int32 ShieldRegenAmount = 2; // 한 주기에 쉴드가 차는 양

private:
	void ResetShieldRegenTimer();
	void StartShieldRegen();
	void RegenerateShield();
	void Die(AActor* DamageCauser);

private:
	FTimerHandle ShieldDelayTimerHandle; // 피격 이후 첫 쉴드가 차기까지의 대기시간 관리 타이머
	FTimerHandle ShieldRegenTimerHandle; // 쉴드가 차는 주기 관리 타이머

	void RestartShieldRegenTimer();
};
