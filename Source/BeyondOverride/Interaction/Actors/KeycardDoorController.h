#pragma once

#include "CoreMinimal.h"

#include "Interaction/InteractableActorBase.h"

#include "KeycardDoorController.generated.h"

UCLASS()
class BEYONDOVERRIDE_API AKeycardDoorController : public AInteractableActorBase
{
	GENERATED_BODY()

  public:
	AKeycardDoorController();

  protected:
	virtual void BeginPlay() override;
	virtual bool CanInteract(AActor* Interactor, FText& OutReason) const override;
	virtual void PerformInteract(AActor* Interactor) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Component")
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(EditInstanceOnly, Category = "KeycardDoor")
	TObjectPtr<AActor> KeyCardDoor;

	UPROPERTY(EditInstanceOnly, Category = "KeycardDoor")
	FName RequiredKeycardID;
};
