#pragma once

#include "CoreMinimal.h"

#include "Interaction/InteractableActorBase.h"

#include "StorageContainerActor.generated.h"

class UInventoryComponent;

UCLASS()
class BEYONDOVERRIDE_API AStorageContainerActor : public AInteractableActorBase
{
	GENERATED_BODY()

  public:
	AStorageContainerActor();

	virtual void BeginPlay() override;

	void SetOpened();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetItems(const TArray<UItemInstanceBase*>& Items);

	UInventoryComponent* GetInventoryComponent()
	{
		return InventoryComponent;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container")
	FName StorageContainerId = "Default";

  protected:
	virtual void PerformInteract(AActor* Interactor) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMesh> OpenedMesh;

  private:
	bool bIsOpened = false;
};
