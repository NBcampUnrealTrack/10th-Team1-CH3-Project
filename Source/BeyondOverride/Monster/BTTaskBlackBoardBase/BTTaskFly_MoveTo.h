// 26/09/09 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

// UHT Header
#include "BTTaskFly_MoveTo.generated.h"

class AMonsterCharacter;

UCLASS()
class BEYONDOVERRIDE_API UBTTaskFly_MoveTo : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
  public:
	UBTTaskFly_MoveTo();

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetLocationKey;

  protected:
	// Task 실행 시 호출되는 함수
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
