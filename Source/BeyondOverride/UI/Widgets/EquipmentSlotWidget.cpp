#include "UI/Widgets/EquipmentSlotWidget.h"

#include "ActorComponents/EquipmentManagerComponent.h"
#include "Components/TextBlock.h"
#include "DataTables/Items/ItemDataRow.h"
#include "Enums/EquipmentSlot.h"
#include "Items/Objects/ItemInstanceBase.h"
#include "Items/Objects/RangeWeaponInstance.h"
#include "Player/ActorComponent/InventoryInteractionComponent.h"
#include "Player/ActorComponent/PlayerInventoryComponent.h"
#include "UI/Widgets/ItemSlotWidget.h"

void UEquipmentSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (WpnNumberInHead)
	{
		WpnNumberInHead->SetText(FText::AsNumber(static_cast<int32>(EquipmentSlot)));
		WpnNumberInBody->SetText(FText::AsNumber(static_cast<int32>(EquipmentSlot)));
	}

	if (ItemSlot)
	{
		ItemSlot->OnSlotClicked.AddDynamic(this, &UEquipmentSlotWidget::HandleItemSlotClicked);
	}
}

void UEquipmentSlotWidget::NativeDestruct()
{
	if (InventoryComponent)
	{
		InventoryComponent->OnEquipmentSlotChanged.RemoveDynamic(this, &UEquipmentSlotWidget::OnEquipmentSlotChanged);
	}

	if (EquipmentManagerComponent)
	{
		EquipmentManagerComponent->OnActiveSlotChangedDelegate.RemoveAll(this);
	}

	Super::NativeDestruct();
}

void UEquipmentSlotWidget::SetupEquipmentSlot(
	UPlayerInventoryComponent* InInventory,
	UInventoryInteractionComponent* InInteraction,
	UEquipmentManagerComponent* InEquipmentManager)
{
	if (InventoryComponent)
	{
		InventoryComponent->OnEquipmentSlotChanged.RemoveDynamic(this, &UEquipmentSlotWidget::OnEquipmentSlotChanged);
	}
	if (EquipmentManagerComponent)
	{
		EquipmentManagerComponent->OnActiveSlotChangedDelegate.RemoveAll(this);
	}

	InventoryComponent = InInventory;
	InteractionComponent = InInteraction;
	EquipmentManagerComponent = InEquipmentManager;

	if (InventoryComponent)
	{
		InventoryComponent->OnEquipmentSlotChanged.AddDynamic(this, &UEquipmentSlotWidget::OnEquipmentSlotChanged);
	}

	if (EquipmentManagerComponent)
	{
		EquipmentManagerComponent->OnActiveSlotChangedDelegate.AddUObject(this, &UEquipmentSlotWidget::OnActiveSlotChanged);
	}

	RefreshItem();
	RefreshEquippedBadge();
}

void UEquipmentSlotWidget::OnEquipmentSlotChanged(EEquipmentSlot ChangedSlot, UItemInstanceBase* ItemInstanceBase)
{
	if (ChangedSlot != EquipmentSlot)
		return;

	RefreshItem();
}

void UEquipmentSlotWidget::OnActiveSlotChanged(EEquipmentSlot ChangedSlot, UEquippableItemInstance* ItemInstance)
{
	RefreshEquippedBadge();
}

void UEquipmentSlotWidget::RefreshEquippedBadge()
{
	if (!WBP_Equipped)
		return;

	const bool bIsActive = EquipmentManagerComponent && EquipmentManagerComponent->GetActiveSlot() == EquipmentSlot;
	WBP_Equipped->SetVisibility(bIsActive ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UEquipmentSlotWidget::RefreshItem()
{
	UItemInstanceBase* Item = InventoryComponent ? InventoryComponent->GetEquipmentItem(EquipmentSlot) : nullptr;

	if (ItemSlot)
	{
		ItemSlot->SetItem(Item, true);
	}

	if (WpnName)
	{
		WpnName->SetText(Item && Item->GetItemData() ? Item->GetItemData()->DisplayName : FText::GetEmpty());
	}

	URangeWeaponInstance* Weapon = Cast<URangeWeaponInstance>(Item);

	if (CurrentAmmoCount )
	{
		CurrentAmmoCount->SetText(Weapon ? FText::AsNumber(Weapon->GetCurrentAmmo()) : FText::GetEmpty());
	}
	if (CurrentAmmoCountInBody )
	{
		CurrentAmmoCountInBody->SetText(Weapon ? FText::AsNumber(Weapon->GetCurrentAmmo()) : FText::GetEmpty());
	}

	if (TotalAmmoCount)
	{
		TotalAmmoCount->SetText(FText::GetEmpty());
	}
	if (TotalAmmoCountInBody)
	{
		TotalAmmoCountInBody->SetText(FText::GetEmpty());
	}
	if (WpnAmmoType)
	{
		WpnAmmoType->SetText(FText::GetEmpty());
	}
}

void UEquipmentSlotWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (CurrentAmmoCount && InventoryComponent)
	{
		if (URangeWeaponInstance* Weapon = Cast<URangeWeaponInstance>(InventoryComponent->GetEquipmentItem(EquipmentSlot)))
		{
			CurrentAmmoCount->SetText(FText::AsNumber(Weapon->GetCurrentAmmo()));
		}
	}
	if (CurrentAmmoCountInBody && InventoryComponent)
	{
		if (URangeWeaponInstance* Weapon = Cast<URangeWeaponInstance>(InventoryComponent->GetEquipmentItem(EquipmentSlot)))
		{
			CurrentAmmoCountInBody->SetText(FText::AsNumber(Weapon->GetCurrentAmmo()));
		}
	}
}

void UEquipmentSlotWidget::HandleItemSlotClicked(int32 SlotIndex, bool bLeftClick)
{
	if (!InteractionComponent || !InventoryComponent)
		return;

	InteractionComponent->HandleEquipmentSlotClick(InventoryComponent, EquipmentSlot, bLeftClick);
}
