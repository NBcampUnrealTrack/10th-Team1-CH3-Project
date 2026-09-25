#pragma once

#include "CoreMinimal.h"

#include "Interaction/InteractableActorBase.h"

#include "KeycardDoorController.generated.h"

class UActorSequenceComponent;

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

	bool HasPlayerKeyCard() const;
	void OpenDoor();

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Component")
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY()
	TObjectPtr<UActorSequenceComponent> DoorSequence;

	bool IsOpened;
};
