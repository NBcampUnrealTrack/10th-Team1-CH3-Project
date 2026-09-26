// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Interaction/InteractableActorBase.h"

#include "AIBuildingExit.generated.h"

UCLASS()
class BEYONDOVERRIDE_API AAIBuildingExit : public AInteractableActorBase
{
	GENERATED_BODY()

  public:
	AAIBuildingExit();

  protected:
	virtual void BeginPlay() override;
	virtual bool CanInteract(AActor* Interactor, FText& OutReason) const override;
	virtual void PerformInteract(AActor* Interactor) override;

	bool IsDefenseStarted() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Component")
	TObjectPtr<UStaticMeshComponent> MeshComp;
};
