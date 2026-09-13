// 26/09/09 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

// UHT Header
#include "BTTaskMakePoint.generated.h"

UCLASS()
class BEYONDOVERRIDE_API UBTTaskMakePoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

  public:
	UBTTaskMakePoint();

  protected:
	// Task 실행 시 호출되는 함수
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
