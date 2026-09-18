#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Interaction/InteractableActorBase.h"

#include "ItemPickupBase.generated.h"

class UItemInstanceBase;

UCLASS()
class AItemPickupBase : public AInteractableActorBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> StaticMeshComp;

	UPROPERTY()
	TObjectPtr<UItemInstanceBase> ItemInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FName ItemID; // 아이템 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (ClampMin = "1", UIMin = "1"))
	int32 StackCount; // 스택 개수

public:
	AItemPickupBase();

	void Initialize(UItemInstanceBase* InItemInstance);

	// Getters
	UItemInstanceBase* GetItemInstance() const;

protected:
	virtual void BeginPlay() override;
	virtual FText GetDisplayTitle(AActor* Interactor) const override;
};
