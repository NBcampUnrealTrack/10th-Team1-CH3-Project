#include "UI/Widgets/InventoryScreenWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Player/ActorComponent/InventoryInteractionComponent.h"
#include "Player/ActorComponent/PlayerInventoryComponent.h"
#include "Player/Character/BOCharacter.h"
#include "UI/Widgets/EquipmentSlotWidget.h"
#include "UI/Widgets/HeldItemWidget.h"
#include "UI/Widgets/ItemSlotPanelWidget.h"

void UInventoryScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ABOCharacter* OwnerCharacter = Cast<ABOCharacter>(GetOwningPlayerPawn());

	if (!OwnerCharacter)
		return;

	if (BackpackSlotPanel)
	{
		BackpackSlotPanel->SetInventory(OwnerCharacter->GetPlayerInventoryComponent(), OwnerCharacter->GetInventoryInteractionComponent());
		BackpackSlotPanel->SetContainerName(FText::FromString(TEXT("가방")));
	}

	if (ContainerSlotPanel)
	{
		ContainerSlotPanel->SetContainerName(FText::FromString(TEXT("주변")));
	}

	if (HeldItem)
	{
		HeldItem->BindInteraction(OwnerCharacter->GetInventoryInteractionComponent());
	}

	if (WidgetTree)
	{
		WidgetTree->ForEachWidget([OwnerCharacter](UWidget* Widget)
								  { 
			if (UEquipmentSlotWidget* EquipmentSlot = Cast<UEquipmentSlotWidget>(Widget)) 
			{
				EquipmentSlot->SetupEquipmentSlot(
					OwnerCharacter->GetPlayerInventoryComponent(),
					OwnerCharacter->GetInventoryInteractionComponent(),
					OwnerCharacter->GetEquipmentComponent());
		} });
	}
}

FReply UInventoryScreenWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	ABOCharacter* OwnerCharacter = Cast<ABOCharacter>(GetOwningPlayerPawn());
	if (OwnerCharacter)
	{
		UInventoryInteractionComponent* Interaction = OwnerCharacter->GetInventoryInteractionComponent();
		if (Interaction && Interaction->IsHoldingItem())
		{
			const FKey EffectingButton = InMouseEvent.GetEffectingButton();
			if (EffectingButton == EKeys::LeftMouseButton || EffectingButton == EKeys::RightMouseButton)
			{
				Interaction->DropItem(EffectingButton == EKeys::LeftMouseButton);
				return FReply::Handled();
			}
		}
	}

	return FReply::Unhandled();
}

void UInventoryScreenWidget::OpenContainer(UInventoryComponent* ContainerInventory, const FText& ContainerName)
{
	if (!ContainerSlotPanel || !ContainerInventory)
		return;

	ABOCharacter* OwnerCharacter = Cast<ABOCharacter>(GetOwningPlayerPawn());
	if (!OwnerCharacter)
		return;

	ContainerSlotPanel->SetInventory(ContainerInventory, OwnerCharacter->GetInventoryInteractionComponent());
	ContainerSlotPanel->SetContainerName(ContainerName);
}

void UInventoryScreenWidget::CloseContainer()
{
	if (!ContainerSlotPanel)
		return;

	ContainerSlotPanel->SetInventory(nullptr, nullptr);
}
