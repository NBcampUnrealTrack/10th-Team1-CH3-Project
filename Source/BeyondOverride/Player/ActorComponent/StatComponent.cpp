#include "Player/ActorComponent/StatComponent.h"

#include "Engine/World.h"
#include "TimerManager.h"

UStatComponent::UStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

	CurHealth = MaxHealth;
	CurShield = MaxShield;
	bIsDead = false;

	OnHealthChanged.Broadcast(CurHealth, MaxHealth);
	OnShieldChanged.Broadcast(CurShield, MaxShield);
}

void UStatComponent::ApplyDamage(int32 DamageAmount)
{
	if (bIsDead || DamageAmount <= 0)
	{
		return;
	}

	// 쉴드 재생 대기 시간 초기화
	ResetShieldRegenTimer();

	if (CurShield > 0) // 쉴드가 있다면 쉴드 깎기
	{
		CurShield = FMath::Clamp(CurShield - DamageAmount, 0, MaxShield);

		OnShieldChanged.Broadcast(CurShield, MaxShield);
	}
	else // 쉴드가 없다면 체력 깎기
	{
		CurHealth = FMath::Clamp(CurHealth - DamageAmount, 0, MaxHealth);

		OnHealthChanged.Broadcast(CurHealth, MaxHealth);

		if (CurHealth <= 0)
		{
			Die();
			return;
		}
	}

	// 쉴드 재생 대기
	if (CurShield < MaxShield)
	{
		GetWorld()->GetTimerManager().SetTimer(ShieldDelayTimerHandle, this, &UStatComponent::StartShieldRegen, ShieldDelayTime, false);
	}
}

void UStatComponent::Heal(int32 HealAmount)
{
	if (bIsDead || HealAmount <= 0)
	{
		return;
	}

	CurHealth = FMath::Clamp(CurHealth + HealAmount, 0, MaxHealth);

	OnHealthChanged.Broadcast(CurHealth, MaxHealth);
}

void UStatComponent::ResetShieldRegenTimer()
{
	if (!GetWorld())
	{
		return;
	}

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	TimerManager.ClearTimer(ShieldDelayTimerHandle);
	TimerManager.ClearTimer(ShieldRegenTimerHandle);
}

void UStatComponent::StartShieldRegen()
{
	if (bIsDead || CurShield >= MaxShield)
	{
		return;
	}

	RegenerateShield();

	if (CurShield < MaxShield)
	{
		GetWorld()->GetTimerManager().SetTimer(ShieldRegenTimerHandle, this, &UStatComponent::RegenerateShield, ShieldRegenTime, true);
	}
}

void UStatComponent::RegenerateShield()
{
	if (bIsDead)
	{
		ResetShieldRegenTimer();
		return;
	}

	CurShield = FMath::Clamp(CurShield + ShieldRegenAmount, 0, MaxShield);

	OnShieldChanged.Broadcast(CurShield, MaxShield);

	// 쉴드가 최대가 되면 더 이상 차지 않음
	if (CurShield >= MaxShield)
	{
		GetWorld()->GetTimerManager().ClearTimer(ShieldRegenTimerHandle);
	}
}

void UStatComponent::Die()
{
	if (bIsDead)
	{
		return;
	}

	bIsDead = true;
	ResetShieldRegenTimer();

	OnDeath.Broadcast();
}

