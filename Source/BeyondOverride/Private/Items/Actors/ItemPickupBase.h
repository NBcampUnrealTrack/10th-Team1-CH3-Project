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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UItemInstanceBase> ItemInstance;

  public:
	AItemPickupBase();

	// Getters
	UItemInstanceBase* GetItemInstance() const;

  protected:
	virtual void BeginPlay() override;
};
