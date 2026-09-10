#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "ItemPickupBase.generated.h"

class UItemInstanceBase;

UCLASS()
class AItemPickupBase : public AActor
{
	GENERATED_BODY()

  protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ID")
	FName ItemID;  // 아이템 ID
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UItemInstanceBase> ItemInstance;

  public:
	AItemPickupBase();

	void Initialize(UItemInstanceBase* InItemInstance);

	// Getters
	UItemInstanceBase* GetItemInstance() const;

  protected:
	virtual void BeginPlay() override;
};
