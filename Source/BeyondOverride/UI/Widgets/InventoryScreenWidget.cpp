#include "UI/Widgets/InventoryScreenWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Player/ActorComponent/InventoryComponent.h"
#include "Player/ActorComponent/InventoryInteractionComponent.h"
#include "Player/Character/BOCharacter.h"
#include "UI/Widgets/ItemSlotPanelWidget.h"

void UInventoryScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ABOCharacter* OwnerCharacter = Cast<ABOCharacter>(GetOwningPlayerPawn());

	if (!OwnerCharacter)
		return;

	if (BackpackSlotPanel)
	{
		BackpackSlotPanel->SetInventory(OwnerCharacter->GetInventoryComponent(), OwnerCharacter->GetInventoryInteractionComponent());
		BackpackSlotPanel->SetContainerName(FText::FromString(TEXT("가방")));
	}

	if (ContainerSlotPanel)
	{
		ContainerSlotPanel->SetContainerName(FText::FromString(TEXT("주변")));
	}
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
