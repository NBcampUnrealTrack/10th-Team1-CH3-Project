#pragma once

#include "CoreMinimal.h"

#include "Interaction/InteractableActorBase.h"

#include "ExitControllerActor.generated.h"

class AExitActor;

UCLASS()
class BEYONDOVERRIDE_API AExitControllerActor : public AInteractableActorBase
{
	GENERATED_BODY()

  public:
	AExitControllerActor();

  protected:
	virtual void BeginPlay() override;

	virtual void PerformInteract(AActor* Interactor) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(EditInstanceOnly, Category = "Extraction")
	TObjectPtr<AExitActor> TargetExit;
};
