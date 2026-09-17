#include "Player/ActorComponent/StatComponent.h"

#include "TimerManager.h"

#include "Engine/World.h"
#include "GameFlow/BOGameInstance.h"

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

void UStatComponent::TakeDamage(int32 DamageAmount, AActor* DamageCauser)
{
	if (bIsDead || DamageAmount <= 0)
	{
		return;
	}

	if (CurShield > 0)
	{
		CurShield = FMath::Clamp(CurShield - DamageAmount, 0, MaxShield);

		OnShieldChanged.Broadcast(CurShield, MaxShield);
	}
	else
	{
		CurHealth = FMath::Clamp(CurHealth - DamageAmount, 0, MaxHealth);

		OnHealthChanged.Broadcast(CurHealth, MaxHealth);
	}

	OnDamaged.Broadcast();

	if (CurHealth <= 0)
	{
		Die(DamageCauser);
		return;
	}

	RestartShieldRegenTimer();
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

void UStatComponent::SetCurHealth(int32 NewCurHealth)
{
	CurHealth = FMath::Clamp(NewCurHealth, 0, MaxHealth);

	OnHealthChanged.Broadcast(CurHealth, MaxHealth);
}

void UStatComponent::SetMaxHealth(int32 NewMaxHealth)
{
	MaxHealth = FMath::Max(NewMaxHealth, 0);
	CurHealth = FMath::Clamp(CurHealth, 0, MaxHealth);

	OnHealthChanged.Broadcast(CurHealth, MaxHealth);
}

void UStatComponent::SetCurShield(int32 NewCurShield)
{
	CurShield = FMath::Clamp(NewCurShield, 0, MaxShield);

	OnShieldChanged.Broadcast(CurShield, MaxShield);

	RestartShieldRegenTimer();
}

void UStatComponent::SetMaxShield(int32 NewMaxShield)
{
	MaxShield = FMath::Max(NewMaxShield, 0);

	CurShield = FMath::Clamp(CurShield, 0, MaxShield);

	OnShieldChanged.Broadcast(CurShield, MaxShield);

	RestartShieldRegenTimer();
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

void UStatComponent::Die(AActor* DamageCauser)
{
	if (bIsDead)
	{
		return;
	}

	bIsDead = true;
	ResetShieldRegenTimer();

	OnDeath.Broadcast(DamageCauser);
}

void UStatComponent::RestartShieldRegenTimer()
{
	if (bIsDead || !GetWorld())
	{
		return;
	}

	ResetShieldRegenTimer();

	if (CurShield >= MaxShield || MaxShield <= 0)
	{
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(ShieldDelayTimerHandle, this, &UStatComponent::StartShieldRegen, ShieldDelayTime, false);
}
