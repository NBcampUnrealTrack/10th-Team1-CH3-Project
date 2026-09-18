#include "UI/Widgets/DamageDisplayWidget.h"

#include "Components/TextBlock.h"

void UDamageDisplayWidget::SetDamageNumber(int32 TotalDamage)
{
	if (DamageText && TotalDamage > 0)
	{
		DamageText->SetText(FText::AsNumber(TotalDamage));
	}
}
