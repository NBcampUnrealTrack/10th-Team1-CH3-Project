// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "CoreMinimal.h"

#include "NPCAIController.generated.h"

UCLASS()
class BEYONDOVERRIDE_API ANPCAIController : public AAIController
{
	GENERATED_BODY()

  public:
	ANPCAIController();

  private:
	virtual void BeginPlay() override;

  public:
	UBlackboardComponent* GetNPCBlackboardComp() const;

  public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	UBlackboardComponent* NPCBlackboardComp;
};
