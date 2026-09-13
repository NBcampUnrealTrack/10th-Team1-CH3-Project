#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "InventoryScreenWidget.generated.h"

class UItemSlotPanelWidget;
class UInventoryComponent;

UCLASS()
class BEYONDOVERRIDE_API UInventoryScreenWidget : public UUserWidget
{
	GENERATED_BODY()

  protected:
	virtual void NativeConstruct() override;

  public:
	void OpenContainer(UInventoryComponent* ContainerInventory, const FText& ContainerName);
	void CloseContainer();

  protected:
	UPROPERTY(meta = (BindWidget))
	UItemSlotPanelWidget* ContainerSlotPanel;

	UPROPERTY(meta = (BindWidget))
	UItemSlotPanelWidget* BackpackSlotPanel;
};
