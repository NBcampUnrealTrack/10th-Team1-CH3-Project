#include "UI/Widgets/HeldItemWidget.h"

#include "Components/CanvasPanelSlot.h"
#include "Items/Objects/ItemInstanceBase.h"
#include "Player/ActorComponent/InventoryInteractionComponent.h"

void UHeldItemWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Collapsed);
}

void UHeldItemWidget::BindInteraction(UInventoryInteractionComponent* InInteraction)
{
	InteractionComponent = InInteraction;
	if (InteractionComponent)
	{
		InteractionComponent->OnHoldItemChanged.AddDynamic(this, &UHeldItemWidget::OnHoldItemChanged);
	}
}

void UHeldItemWidget::OnHoldItemChanged(const UItemInstanceBase* HoldItem)
{
	SetItem(const_cast<UItemInstanceBase*>(HoldItem));
	SetVisibility(HoldItem ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UHeldItemWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (GetVisibility() == ESlateVisibility::Collapsed)
		return;

	APlayerController* PC = GetOwningPlayer();
	if (!PC)
		return;

	float MouseX = 0.f, MouseY = 0.f;
	if (PC->GetMousePosition(MouseX, MouseY))
	{
		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot))
		{
			CanvasSlot->SetPosition(FVector2D(MouseX, MouseY));
		}
	}
}
