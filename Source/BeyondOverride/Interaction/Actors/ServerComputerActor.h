// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Interaction/InteractableActorBase.h"

#include "ServerComputerActor.generated.h"

/**
 *
 */
UCLASS()
class BEYONDOVERRIDE_API AServerComputerActor : public AInteractableActorBase
{
	GENERATED_BODY()

  public:
	AServerComputerActor();

  protected:
	virtual void PerformInteract(AActor* Interactor) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UStaticMeshComponent> MeshComp;
};
