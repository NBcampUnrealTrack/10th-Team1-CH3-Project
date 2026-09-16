#include "UI/Widgets/DamageFlashWidget.h"

#include "Player/ActorComponent/StatComponent.h"
#include "Player/Character/BOCharacter.h"

void UDamageFlashWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ABOCharacter* OwningCharacter = Cast<ABOCharacter>(GetOwningPlayerPawn());
	if (!OwningCharacter)
		return;

	StatComponent = OwningCharacter->GetStatComponent();
	if (!StatComponent)
		return;

	LastShield = StatComponent->GetCurShield();
	LastHealth = StatComponent->GetCurHealth();

	StatComponent->OnShieldChanged.AddDynamic(this, &UDamageFlashWidget::HandleShieldChanged);
	StatComponent->OnHealthChanged.AddDynamic(this, &UDamageFlashWidget::HandleHealthChanged);
}

void UDamageFlashWidget::NativeDestruct()
{
	if (StatComponent)
	{
		StatComponent->OnShieldChanged.RemoveDynamic(this, &UDamageFlashWidget::HandleShieldChanged);
		StatComponent->OnHealthChanged.RemoveDynamic(this, &UDamageFlashWidget::HandleHealthChanged);
		StatComponent = nullptr;
	}

	Super::NativeDestruct();
}

void UDamageFlashWidget::HandleShieldChanged(int32 CurShield, int32 MaxShield)
{
	if (CurShield < LastShield)
	{
		PlayShieldFlash();
	}

	LastShield = CurShield;
}

void UDamageFlashWidget::HandleHealthChanged(int32 CurHealth, int32 MaxHealth)
{
	if (CurHealth < LastHealth)
	{
		PlayHealthFlash();
	}

	LastHealth = CurHealth;
}
