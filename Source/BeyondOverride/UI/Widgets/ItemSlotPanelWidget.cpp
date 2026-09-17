#include "UI/Widgets/ItemSlotPanelWidget.h"

#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Items/Actors/ItemPickupBase.h"
#include "Items/Objects/ItemInstanceBase.h"
#include "Player/ActorComponent/InventoryComponent.h"
#include "Player/ActorComponent/InventoryInteractionComponent.h"
#include "Player/ActorComponent/NearbyItemComponent.h"
#include "UI/Widgets/ItemSlotWidget.h"
#include "UI/Widgets/PanelFrameWidget.h"
#include "UObject/ConstructorHelpers.h"

UItemSlotPanelWidget::UItemSlotPanelWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UItemSlotWidget> SlotWBPClass(TEXT("/Game/UI/Widgets/WBP_ItemSlot"));
	if (SlotWBPClass.Succeeded())
	{
		SlotWidgetClass = SlotWBPClass.Class;
	}
}

void UItemSlotPanelWidget::NativeDestruct()
{
	UnbindInventory();
	Super::NativeDestruct();
}

FReply UItemSlotPanelWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (Mode == EItemSlotPanelMode::WorldItems && InteractionComponent && InteractionComponent->IsHoldingItem())
	{
		const FKey EffectingButton = InMouseEvent.GetEffectingButton();

		if (EffectingButton == EKeys::LeftMouseButton || EffectingButton == EKeys::RightMouseButton)
		{
			InteractionComponent->DropItem(EffectingButton == EKeys::LeftMouseButton);
		}
	}

	return FReply::Handled();
}

void UItemSlotPanelWidget::UnbindInventory()
{
	if (InventoryComponent)
	{
		InventoryComponent->OnInventoryChanged.RemoveDynamic(this, &UItemSlotPanelWidget::OnInventoryChanged);
		InventoryComponent = nullptr;
	}
}

void UItemSlotPanelWidget::SetInventory(UInventoryComponent* InInventory, UInventoryInteractionComponent* InInteraction)
{
	UnbindInventory();
	WorldItems.Empty();

	Mode = EItemSlotPanelMode::Inventory;
	InventoryComponent = InInventory;
	InteractionComponent = InInteraction;

	if (InventoryComponent)
	{
		InventoryComponent->OnInventoryChanged.AddDynamic(this, &UItemSlotPanelWidget::OnInventoryChanged);
	}

	RefreshSlots();
}

void UItemSlotPanelWidget::SetWorldItems(const TArray<AItemPickupBase*>& InItems, UNearbyItemComponent* InNearbyItemComponent, UInventoryInteractionComponent* InInteraction)
{
	UnbindInventory();

	Mode = EItemSlotPanelMode::WorldItems;
	WorldItems = InItems;
	NearbyItemComponent = InNearbyItemComponent;
	InteractionComponent = InInteraction;

	RefreshSlots();
}

void UItemSlotPanelWidget::OnInventoryChanged(const TArray<UItemInstanceBase*>& Slots)
{
	RefreshSlots();
}

void UItemSlotPanelWidget::SetContainerName(const FText& InName)
{
	if (PanelFrame)
		PanelFrame->SetContainerName(InName);
}

void UItemSlotPanelWidget::RefreshSlots()
{
	if (!SlotContainer || !SlotWidgetClass)
		return;

	SlotContainer->ClearChildren();

	int32 SlotCount = 0;
	if (Mode == EItemSlotPanelMode::Inventory)
	{
		if (!InventoryComponent)
			return;
		SlotCount = InventoryComponent->GetSlotCount();
	}
	else
	{
		SlotCount = WorldItems.Num();
	}

	int32 CurrentCount = 0;

	for (int32 Index = 0; Index < SlotCount; Index++)
	{
		UItemInstanceBase* Item = nullptr;
		if (Mode == EItemSlotPanelMode::Inventory)
		{
			Item = InventoryComponent->GetItem(Index);
		}
		else if (WorldItems.IsValidIndex(Index) && WorldItems[Index])
		{
			 Item = WorldItems[Index]->GetItemInstance();
		}

		if (Item)
			CurrentCount++;

		UItemSlotWidget* SlotWidget = CreateWidget<UItemSlotWidget>(this, SlotWidgetClass);
		if (!SlotWidget)
			continue;

		SlotWidget->SetSlotIndex(Index);
		SlotWidget->SetItem(Item);
		SlotWidget->OnSlotClicked.AddDynamic(this, &UItemSlotPanelWidget::HandleSlotClicked);

		const int32 Row = Index / ColumnCount;
		const int32 Column = Index % ColumnCount;

		SlotContainer->AddChildToUniformGrid(SlotWidget, Row, Column);
	}

	if (PanelFrame)
		PanelFrame->SetSlotCount(CurrentCount, SlotCount);
}

void UItemSlotPanelWidget::HandleSlotClicked(int32 SlotIndex, bool bLeftClick)
{
	if (!InteractionComponent)
		return;

	if (Mode == EItemSlotPanelMode::Inventory)
	{
		if (!InventoryComponent)
			return;
		InteractionComponent->HandleSlotClick(InventoryComponent, SlotIndex, bLeftClick);
	}
	else
	{
		InteractionComponent->HandleNearbySlotClick(NearbyItemComponent, SlotIndex, bLeftClick);
	}
}
