// 26/09/09 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

// UHT Header
#include "BTTaskOverlapCheck.generated.h"

class ABOCharacter;

UCLASS()
class BEYONDOVERRIDE_API UBTTaskOverlapCheck : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

  public:
	UBTTaskOverlapCheck();

  protected:
	// Task 실행 시 호출되는 함수
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void OverlapAndTraceCheck(UBehaviorTreeComponent& OwnerComp, ABOCharacter*& NearestTarget);
};
