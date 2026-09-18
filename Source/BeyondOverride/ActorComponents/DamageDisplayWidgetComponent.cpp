#include "ActorComponents/DamageDisplayWidgetComponent.h"

#include "UI/Widgets/DamageDisplayWidget.h"

UDamageDisplayWidgetComponent::UDamageDisplayWidgetComponent()
{
	SetWidget(DamageDisplayWidget);
	SetWidgetSpace(EWidgetSpace::Screen);

	TotalDamage = 0;
	DamageAccumulationDuration = 1;
}

void UDamageDisplayWidgetComponent::TakeDamage(int32 Damage)
{
	TotalDamage += Damage;
	if (DamageDisplayWidget)
	{
		DamageDisplayWidget->SetDamageNumber(TotalDamage);
		DamageDisplayWidget->SetVisibility(ESlateVisibility::Visible);
	}

	GetWorld()->GetTimerManager().SetTimer(
		DamageAccumulationTimerHandle,
		this,
		&UDamageDisplayWidgetComponent::OnDamageAccumulationEnded,
		DamageAccumulationDuration,
		false);
}

void UDamageDisplayWidgetComponent::OnDamageAccumulationEnded()
{
	TotalDamage = 0;
	if (DamageDisplayWidget)
	{
		DamageDisplayWidget->SetDamageNumber(TotalDamage);
		DamageDisplayWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}
