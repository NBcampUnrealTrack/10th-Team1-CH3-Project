// 26/09/13 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "BehaviorTree/Tasks/BTTask_RunEQSQuery.h"

// UHT Header
#include "BTTaskEQSQuery.generated.h"

UCLASS()
class BEYONDOVERRIDE_API UBTTaskEQSQuery : public UBTTask_RunEQSQuery
{
	GENERATED_BODY()

  public:
	UBTTaskEQSQuery();

  protected:
	// Task 실행 시 호출되는 함수
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
