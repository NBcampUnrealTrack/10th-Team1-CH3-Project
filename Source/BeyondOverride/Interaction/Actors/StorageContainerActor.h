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

	void SetOpened();

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
