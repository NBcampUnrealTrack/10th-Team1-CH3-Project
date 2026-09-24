#include "UI/Widgets/HeldItemWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
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
	UpdatePosition();
}

void UHeldItemWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (GetVisibility() == ESlateVisibility::Collapsed)
		return;

	UpdatePosition();
}

void UHeldItemWidget::UpdatePosition()
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC)
		return;

	float MouseX = 0.f, MouseY = 0.f;

	// PC->GetMousePosition(MouseX, MouseY)
	//  UMG가 계산하는 DPI 스케일 값이 달라져서 Standalone에서는 잘 맞아보이지만
	//  PIE모드는 마우스 좌표와 실제 위젯 좌표사이에 오차가 생긴다

	UWidgetLayoutLibrary::GetMousePositionScaledByDPI(PC, MouseX, MouseY);

	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot))
	{
		CanvasSlot->SetPosition(FVector2D(MouseX + 20.f, MouseY + 20.f));
	}
}
