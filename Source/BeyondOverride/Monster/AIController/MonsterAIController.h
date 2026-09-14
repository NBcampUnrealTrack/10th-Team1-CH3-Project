// 26/09/09 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "AIController.h"

// Add include
#include "Perception/AIPerceptionTypes.h"

// UHT Header
#include "MonsterAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;

UCLASS()
class BEYONDOVERRIDE_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()

  public:
	// 생성자
	AMonsterAIController();

	// BehaviorTree 시작 함수
	void EnableBehaviorTree();

  protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Moster|AI")
	TObjectPtr<UAIPerceptionComponent> AIPerception;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Moster|AI")
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Moster|AI")
	TObjectPtr<UAISenseConfig_Hearing> HearingConfig;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTargetHearUpdated(AActor* Actor, FAIStimulus Stimulus);

	// AI Controller가 Pawn 조종 시작시의 함수, override를 통해 재정의
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditDefaultsOnly, Category = "Monster|AI")
	class UBehaviorTree* BehaviorTreeAsset;
};
