#include "MainScreenWidget.h"

#include "Components/ProgressBar.h"
#include "Player/ActorComponent/StatComponent.h"
#include "Player/Character/BOCharacter.h"

void UMainScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ABOCharacter* Character = Cast<ABOCharacter>(GetOwningPlayerPawn());
	if (!Character)
		return;

	StatComponent = Character->GetStatComponent();
	if (!StatComponent)
		return;

	StatComponent->OnHealthChanged.AddDynamic(this, &UMainScreenWidget::HandleHealthChanged);
	StatComponent->OnShieldChanged.AddDynamic(this, &UMainScreenWidget::HandleShieldChanged);

	HandleHealthChanged(StatComponent->GetCurHealth(), StatComponent->GetMaxHealth());
	HandleShieldChanged(StatComponent->GetCurShield(), StatComponent->GetMaxShield());
}

void UMainScreenWidget::NativeDestruct()
{
	if (StatComponent)
	{
		StatComponent->OnHealthChanged.RemoveDynamic(this, &UMainScreenWidget::HandleHealthChanged);
		StatComponent->OnShieldChanged.RemoveDynamic(this, &UMainScreenWidget::HandleShieldChanged);
		StatComponent = nullptr;
	}

	Super::NativeDestruct();
}

void UMainScreenWidget::HandleHealthChanged(int32 Health, int32 MaxHealth)
{
	if (!HealthBar)
		return;

	HealthBar->SetPercent(static_cast<float>(Health) / static_cast<float>(MaxHealth));
}

void UMainScreenWidget::HandleShieldChanged(int32 Shield, int32 MaxShield)
{
	if (!ShieldBar)
		return;

	ShieldBar->SetPercent(static_cast<float>(Shield) / static_cast<float>(MaxShield));
}
