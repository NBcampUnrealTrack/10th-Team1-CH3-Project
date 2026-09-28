#include "UI/Widgets/InventoryScreenWidget.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Items/Actors/ItemPickupBase.h"
#include "Player/ActorComponent/CharacterPreviewComponent.h"
#include "Player/ActorComponent/InventoryInteractionComponent.h"
#include "Player/ActorComponent/NearbyItemComponent.h"
#include "Player/ActorComponent/PlayerInventoryComponent.h"
#include "Player/Character/BOCharacter.h"
#include "UI/Widgets/EquipmentSlotWidget.h"
#include "UI/Widgets/HeldItemWidget.h"
#include "UI/Widgets/ItemSlotPanelWidget.h"
#include "UI/Widgets/ItemTooltipWidget.h"
#include "Items/Objects/ItemInstanceBase.h"

void UInventoryScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ABOCharacter* OwnerCharacter = Cast<ABOCharacter>(GetOwningPlayerPawn());

	if (!OwnerCharacter)
		return;

	if (CharacterPreviewImage)
	{
		if (UCharacterPreviewComponent* Preview = OwnerCharacter->GetCharacterPreviewComponent())
		{
			Preview->SetPreviewActive(true);
		}
	}

	if (BackpackSlotPanel)
	{
		BackpackSlotPanel->SetInventory(OwnerCharacter->GetPlayerInventoryComponent(), OwnerCharacter->GetInventoryInteractionComponent());
		BackpackSlotPanel->SetContainerName(FText::FromString(TEXT("가방")));
		BackpackSlotPanel->OnSlotHoverChanged.AddDynamic(this, &UInventoryScreenWidget::HandleSlotHovered);
	}

	if (ContainerSlotPanel)
	{
		ContainerSlotPanel->SetContainerName(FText::FromString(TEXT("주변")));

		if (UNearbyItemComponent* PlayerNearbyItemComponent = OwnerCharacter->GetNearbyItemComponent())
		{
			PlayerNearbyItemComponent->OnNearbyItemsChanged.AddDynamic(this, &UInventoryScreenWidget::OnNearbyItemsChanged);
			ContainerSlotPanel->SetWorldItems(PlayerNearbyItemComponent->GetItemPickups(), PlayerNearbyItemComponent, OwnerCharacter->GetInventoryInteractionComponent());
			ContainerSlotPanel->OnSlotHoverChanged.AddDynamic(this, &UInventoryScreenWidget::HandleSlotHovered);
		}
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

	if (UInventoryInteractionComponent* IIC = OwnerCharacter->GetInventoryInteractionComponent())
	{
		IIC->OnHoldItemChanged.AddDynamic(this, &UInventoryScreenWidget::HandleHoldItemChanged);
	}
}

FReply UInventoryScreenWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (EquipmentSlotArea)
	{
		const FGeometry& AreaGeometry = EquipmentSlotArea->GetCachedGeometry();

		if (USlateBlueprintLibrary::IsUnderLocation(AreaGeometry, InMouseEvent.GetScreenSpacePosition()))
		{
			return FReply::Handled();
		}
	}

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

void UInventoryScreenWidget::NativeDestruct()
{
	APawn* OwningPawn = GetOwningPlayerPawn();
	ABOCharacter* Character = Cast<ABOCharacter>(OwningPawn);

	if (IsValid(Character))
	{
		UInventoryInteractionComponent* InteractionComponent = Character->GetInventoryInteractionComponent();

		if (IsValid(InteractionComponent) && InteractionComponent->IsHoldingItem())
		{
			// 손에 들고 있는 아이템 전부 버리기
			InteractionComponent->DropItem(true);
		}
	}

	ABOCharacter* OwnerCharacter = Cast<ABOCharacter>(GetOwningPlayerPawn());
	if (OwnerCharacter)
	{
		if (UCharacterPreviewComponent* Preview = OwnerCharacter->GetCharacterPreviewComponent())
		{
			Preview->SetPreviewActive(false);
		}

		if (UNearbyItemComponent* PlayerNearbyItemComponent = OwnerCharacter->GetNearbyItemComponent())
		{
			PlayerNearbyItemComponent->OnNearbyItemsChanged.RemoveDynamic(this, &UInventoryScreenWidget::OnNearbyItemsChanged);
		}
	}

	Super::NativeDestruct();
}

void UInventoryScreenWidget::OpenContainer(UInventoryComponent* ContainerInventory, const FText& ContainerName)
{
	if (!ContainerSlotPanel || !ContainerInventory)
		return;

	ABOCharacter* OwnerCharacter = Cast<ABOCharacter>(GetOwningPlayerPawn());
	if (!OwnerCharacter)
		return;

	bIsContainerOpen = true;

	ContainerSlotPanel->SetInventory(ContainerInventory, OwnerCharacter->GetInventoryInteractionComponent());
	ContainerSlotPanel->SetContainerName(ContainerName);
}

void UInventoryScreenWidget::CloseContainer()
{
	if (!ContainerSlotPanel)
		return;

	bIsContainerOpen = false;

	ContainerSlotPanel->SetInventory(nullptr, nullptr);
}

void UInventoryScreenWidget::OnNearbyItemsChanged(const TArray<AItemPickupBase*>& NearbyItems)
{
	if (!ContainerSlotPanel || bIsContainerOpen)
		return;

	ABOCharacter* OwnerCharacter = Cast<ABOCharacter>(GetOwningPlayerPawn());
	if (!OwnerCharacter)
		return;

	ContainerSlotPanel->SetWorldItems(NearbyItems, OwnerCharacter->GetNearbyItemComponent(), OwnerCharacter->GetInventoryInteractionComponent());
}

void UInventoryScreenWidget::HandleSlotHovered(bool bIsHovered, UItemInstanceBase* Item)
{
	if (!Tooltip || HeldItem->GetVisibility() != ESlateVisibility::Collapsed)
		return;

	Tooltip->OnItemHovered(bIsHovered, Item);
}

void UInventoryScreenWidget::HandleHoldItemChanged(const UItemInstanceBase* HoldItem)
{
	if (HoldItem && Tooltip)
	{
		Tooltip->OnItemHovered(false, nullptr);
	}
}
