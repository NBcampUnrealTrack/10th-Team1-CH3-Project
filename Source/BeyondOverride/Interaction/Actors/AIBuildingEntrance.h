// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Interaction/InteractableActorBase.h"

#include "AIBuildingEntrance.generated.h"

UCLASS()
class BEYONDOVERRIDE_API AAIBuildingEntrance : public AInteractableActorBase
{
	GENERATED_BODY()

  public:
	AAIBuildingEntrance();

  protected:
	virtual void BeginPlay() override;
	virtual bool CanInteract(AActor* Interactor, FText& OutReason) const override;
	virtual void PerformInteract(AActor* Interactor) override;

	bool IsBossDefeated() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Component")
	TObjectPtr<UStaticMeshComponent> MeshComp;
};
