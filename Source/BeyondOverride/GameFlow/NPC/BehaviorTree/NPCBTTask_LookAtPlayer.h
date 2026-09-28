// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

#include "NPCBTTask_LookAtPlayer.generated.h"

UCLASS()
class BEYONDOVERRIDE_API UNPCBTTask_LookAtPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

  public:
	UNPCBTTask_LookAtPlayer();

  public:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float RotationSpeed;
};
