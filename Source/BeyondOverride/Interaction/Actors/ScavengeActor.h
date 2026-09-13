#pragma once

#include "CoreMinimal.h"

#include "Interaction/InteractableActorBase.h"

#include "ScavengeActor.generated.h"

UCLASS()
class BEYONDOVERRIDE_API AScavengeActor : public AInteractableActorBase
{
	GENERATED_BODY()

  public:
	AScavengeActor();

  protected:
	virtual void PerformInteract(AActor* Interactor) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UStaticMeshComponent> MeshComp;
};
