// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

#include "NPCBTTask_FindRandomLocation.generated.h"

UCLASS()
class BEYONDOVERRIDE_API UNPCBTTask_FindRandomLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

  public:
	UNPCBTTask_FindRandomLocation();

  public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector LocationKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float SearchRadius = 1000.0f;
};
