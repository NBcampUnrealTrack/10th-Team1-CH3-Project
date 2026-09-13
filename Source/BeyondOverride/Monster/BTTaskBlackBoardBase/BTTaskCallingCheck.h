// 26/09/12 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

// UHT Header
#include "BTTaskCallingCheck.generated.h"

UCLASS()
class BEYONDOVERRIDE_API UBTTaskCallingCheck : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

  public:
	UBTTaskCallingCheck();

  protected:
	// Task 실행 시 호출되는 함수
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
