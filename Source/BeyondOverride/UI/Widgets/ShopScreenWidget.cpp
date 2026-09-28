#include "UI/Widgets/ShopScreenWidget.h"

#include "Components/TextBlock.h"
#include "Player/ActorComponent/InventoryInteractionComponent.h"
#include "Player/ActorComponent/PlayerInventoryComponent.h"
#include "Player/Character/BOCharacter.h"
#include "UI/Manager/UIManager.h"
#include "UI/Widgets/HeldItemWidget.h"
#include "UI/Widgets/ItemSlotPanelWidget.h"
#include "UI/Widgets/ItemTooltipWidget.h"

void UShopScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ABOCharacter* OwnerCharacter = Cast<ABOCharacter>(GetOwningPlayerPawn());
	if (!OwnerCharacter)
		return;

	Inventory = OwnerCharacter->GetPlayerInventoryComponent();

	if (Inventory)
	{
		Inventory->OnMoneyChanged.AddDynamic(this, &UShopScreenWidget::HandleMoneyChanged);

		HandleMoneyChanged(Inventory->GetMoney());
	}

	if (BackpackSlotPanel)
	{
		BackpackSlotPanel->SetInventory(OwnerCharacter->GetPlayerInventoryComponent(), OwnerCharacter->GetInventoryInteractionComponent());
		BackpackSlotPanel->SetContainerName(FText::FromString(TEXT("가방")));
		BackpackSlotPanel->OnSlotHoverChanged.AddDynamic(this, &UShopScreenWidget::HandleSlotHovered);
	}

	if (ShopSlotPanel)
	{
		ShopSlotPanel->SetShopItems(TArray<UItemInstanceBase*>(), OwnerCharacter->GetInventoryInteractionComponent());
		ShopSlotPanel->SetContainerName(FText::FromString(TEXT("상점")));
		ShopSlotPanel->OnSlotHoverChanged.AddDynamic(this, &UShopScreenWidget::HandleSlotHovered);
	}

	if (HeldItem)
	{
		HeldItem->BindInteraction(OwnerCharacter->GetInventoryInteractionComponent());
	}
	
	if (UInventoryInteractionComponent* IIC = OwnerCharacter->GetInventoryInteractionComponent())
	{
		IIC->OnHoldItemChanged.AddDynamic(this, &UShopScreenWidget::HandleHoldItemChanged);
	}
}

void UShopScreenWidget::NativeDestruct()
{
	APawn* OwningPawn = GetOwningPlayerPawn();
	ABOCharacter* Character = Cast<ABOCharacter>(OwningPawn);

	if (IsValid(Character))
	{
		UInventoryInteractionComponent* InteractionComponent = Character->GetInventoryInteractionComponent();

		if (IsValid(InteractionComponent) && InteractionComponent->IsHoldingItem())
		{
			InteractionComponent->DropItem(true);
		}
	}

	if (Inventory)
	{
		Inventory->OnMoneyChanged.RemoveDynamic(this, &UShopScreenWidget::HandleMoneyChanged);
		Inventory = nullptr;
	}

	if (BackpackSlotPanel)
	{
		BackpackSlotPanel->OnSlotHoverChanged.RemoveDynamic(this, &UShopScreenWidget::HandleSlotHovered);
		BackpackSlotPanel = nullptr;
	}

	if (ShopSlotPanel)
	{
		ShopSlotPanel->OnSlotHoverChanged.RemoveDynamic(this, &UShopScreenWidget::HandleSlotHovered);
		ShopSlotPanel = nullptr;
	}

	Super::NativeDestruct();
}

void UShopScreenWidget::HandleSlotHovered(bool bIsHovered, UItemInstanceBase* Item)
{
	if (!Tooltip || HeldItem->GetVisibility() != ESlateVisibility::Collapsed)
		return;

	Tooltip->OnItemHovered(bIsHovered, Item);
}

FReply UShopScreenWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Escape)
	{
		UUIManager* UIManager = UUIManager::Get(this);
		UIManager->PopScreen();
	}

	return FReply::Handled();
}

void UShopScreenWidget::SetShopItems(const TArray<UItemInstanceBase*>& InItems)
{
	CachedShopItems.Empty();
	for (UItemInstanceBase* Item : InItems)
	{
		CachedShopItems.Add(Item);
	}

	ABOCharacter* OwnerCharacter = Cast<ABOCharacter>(GetOwningPlayerPawn());
	if (ShopSlotPanel && OwnerCharacter)
	{
		ShopSlotPanel->SetShopItems(InItems, OwnerCharacter->GetInventoryInteractionComponent());
	}
}

void UShopScreenWidget::HandleMoneyChanged(int32 Money)
{
	if (TXT_Money)
		TXT_Money->SetText(FText::AsNumber(Money));
}

void UShopScreenWidget::HandleHoldItemChanged(const UItemInstanceBase* HoldItem)
{
	if (HoldItem && Tooltip)
	{
		Tooltip->OnItemHovered(false, nullptr);
	}
}
