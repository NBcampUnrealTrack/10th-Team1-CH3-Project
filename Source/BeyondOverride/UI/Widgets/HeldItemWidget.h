#pragma once

#include "CoreMinimal.h"

#include "UI/Widgets/ItemSlotWidget.h"

#include "HeldItemWidget.generated.h"

class UInventoryInteractionComponent;

UCLASS()
class BEYONDOVERRIDE_API UHeldItemWidget : public UItemSlotWidget
{
	GENERATED_BODY()

  public:
	void BindInteraction(UInventoryInteractionComponent* InInteraction);

  protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

  private:
	UPROPERTY()
	TObjectPtr<UInventoryInteractionComponent> InteractionComponent;

	UFUNCTION()
	void OnHoldItemChanged(const UItemInstanceBase* HoldItem);

	void UpdatePosition();
};
