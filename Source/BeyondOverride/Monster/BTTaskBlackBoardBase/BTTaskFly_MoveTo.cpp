// 26/09/25 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/BTTaskBlackBoardBase/BTTaskFly_MoveTo.h"

// Add include
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "Monster/AiController/MonsterAIController.h"
#include "Monster/Enums/InfoEnums.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"
#include "Monster/Structs/SystemParams.h"

UBTTaskFly_MoveTo::UBTTaskFly_MoveTo()
{
	NodeName = TEXT("Fly_MoveTo");
	TargetLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTaskFly_MoveTo, TargetLocationKey));
}

void UBTTaskFly_MoveTo::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BlackboardAsset = GetBlackboardAsset();

	if (BlackboardAsset)
	{
		TargetLocationKey.ResolveSelectedKey(*BlackboardAsset);
	}
}

EBTNodeResult::Type UBTTaskFly_MoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	AMonsterAIController* AIController = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	AMonsterCharacter* AIMonster = AIController->GetMonster();
	if (!AIMonster)
	{
		return EBTNodeResult::Failed;
	}

	if (!TargetLocationKey.IsSet())
	{
		return EBTNodeResult::Failed;
	}

	if (!BlackboardComp->IsVectorValueSet(TargetLocationKey.SelectedKeyName))
	{
		return EBTNodeResult::Failed;
	}

	FVector TargetLocation = BlackboardComp->GetValueAsVector(TargetLocationKey.SelectedKeyName);

	TargetLocation.Z = FMath::FRandRange(TargetLocation.Z + AIMonster->GetFlyMin(),
										 TargetLocation.Z + AIMonster->GetFlyMax());

	if (AIController->AirNavControl(TargetLocation))
	{
		AIController->MoveFlying(AIController->AirNavResult());
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
