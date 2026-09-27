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
#include "Player/ActorComponent/PlayerInventoryComponent.h"

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
	if (InteractionComponent && InteractionComponent->IsHoldingItem())
	{
		const FKey EffectingButton = InMouseEvent.GetEffectingButton();
		const bool bIsMouseButton = EffectingButton == EKeys::LeftMouseButton || EffectingButton == EKeys::RightMouseButton;

		if (bIsMouseButton)
		{
			const bool bLeftClick = EffectingButton == EKeys::LeftMouseButton;

			if (Mode == EItemSlotPanelMode::WorldItems)
				InteractionComponent->DropItem(bLeftClick);
			else if (Mode == EItemSlotPanelMode::Shop)
				InteractionComponent->SellItem(bLeftClick);
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

	if (PlayerInventoryComponent)
	{
		PlayerInventoryComponent->OnWeightChanged.RemoveDynamic(this, &UItemSlotPanelWidget::OnWeightChanged);
		PlayerInventoryComponent = nullptr;
	}

	if (PanelFrame)
	{
		PanelFrame->HideCarryWeight();
		PanelFrame->HideCountTextBox();
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

	PlayerInventoryComponent = Cast<UPlayerInventoryComponent>(InInventory);
	if (PlayerInventoryComponent)
	{
		PlayerInventoryComponent->OnWeightChanged.AddDynamic(this, &UItemSlotPanelWidget::OnWeightChanged);
		OnWeightChanged(PlayerInventoryComponent->GetCurCarryWeight(), PlayerInventoryComponent->GetMaxCarryWeight());
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

void UItemSlotPanelWidget::SetShopItems(const TArray<UItemInstanceBase*>& InItems, UInventoryInteractionComponent* InInteraction)
{
	UnbindInventory();
	WorldItems.Empty();

	Mode = EItemSlotPanelMode::Shop;
	ShopItems.Empty();
	for (UItemInstanceBase* Item : InItems)
		ShopItems.Add(Item);
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
	else if (Mode == EItemSlotPanelMode::Shop)
	{
		SlotCount = ShopItems.Num();
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
		else if (Mode == EItemSlotPanelMode::Shop)
		{
			Item = ShopItems.IsValidIndex(Index) ? ShopItems[Index].Get() : nullptr;
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
		SlotWidget->OnSlotHovered.AddDynamic(this, &UItemSlotPanelWidget::HandleSlotHovered);
		
		const int32 Row = Index / ColumnCount;
		const int32 Column = Index % ColumnCount;

		SlotContainer->AddChildToUniformGrid(SlotWidget, Row, Column);
	}

	if (PanelFrame && Mode == EItemSlotPanelMode::Inventory)
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
	else if (Mode == EItemSlotPanelMode::Shop)
	{
		if (InteractionComponent->IsHoldingItem())
		{
			InteractionComponent->SellItem(bLeftClick);
			return;
		}

		if (!ShopItems.IsValidIndex(SlotIndex) || !ShopItems[SlotIndex])
			return;

		UItemInstanceBase* BuyCopy = DuplicateObject<UItemInstanceBase>(ShopItems[SlotIndex], InteractionComponent->GetOwner());
		if (!BuyCopy)
			return;

		BuyCopy->Initialize();
		BuyCopy->SetStackCount(1);

		InteractionComponent->BuyItem(BuyCopy);
	}
	else
	{
		InteractionComponent->HandleNearbySlotClick(NearbyItemComponent, SlotIndex, bLeftClick);
	}
}

void UItemSlotPanelWidget::OnWeightChanged(float CurCarryWeight, float MaxCarryWeight)
{
	if (PanelFrame)
	{	
		PanelFrame->SetCarryWeight(CurCarryWeight, MaxCarryWeight);
	}
}

void UItemSlotPanelWidget::HandleSlotHovered(bool bIsHovered, UItemInstanceBase* SlotData)
{
	OnSlotHoverChanged.Broadcast(bIsHovered, SlotData);
}
