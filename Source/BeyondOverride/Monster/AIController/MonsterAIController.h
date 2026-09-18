// 26/09/09 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "AIController.h"

// Add include
#include "Monster/ActorComponent/ContinuousStateComponent.h"
#include "Monster/ActorComponent/ShortTermStateComponent.h"
#include "Perception/AIPerceptionTypes.h"

// UHT Header
#include "MonsterAIController.generated.h"

// 전방 선언
class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class UContinuousStateComponent;
class UShortTermStateComponent;
class USenseComponent;
class ABOCharacter;

UCLASS()
class BEYONDOVERRIDE_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()

	// Methtods
  public:
	// 생성자
	AMonsterAIController();

	// BehaviorTree 시작 함수
	void EnableBehaviorTree();

	void PlantFlag(FFlagInfo FlagInfo);

	void PlantFlag(EFlag State, float Time);

	void PlantFlag(EFlag State, float Time, bool Type);

	bool FoldFlags(EFlag Target);

	bool FoldFlags(EFlag Target, bool& Type);

	void StateChange(EMonsterState Input);

	void StateChange(EMonsterState Input, float HoldTime);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MonsterState")
	EMonsterState GetState() const;

	EMonsterState GetBeforeState() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MonsterState")
	bool IsContinueState() const;

	void SetTarget(ABOCharacter* Target);
	ABOCharacter* GetTarget() const;

	void SetTargetPoint(FVector Point);
	FVector GetTargetPoint() const;

	FVector GetSpawnPoint() const;

	void FocusSetUp(const EMonsterState& input);

  protected:
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnTargetHearUpdated(AActor* Actor, FAIStimulus Stimulus);

	// AI Controller가 Pawn 조종 시작시의 함수, override를 통해 재정의
	virtual void OnPossess(APawn* InPawn) override;

	// Properties
  public:
  protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI|Control")
	class UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|State")
	TObjectPtr<UContinuousStateComponent> State;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|State")
	TObjectPtr<UShortTermStateComponent> Flag;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|State")
	TObjectPtr<USenseComponent> SenseValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Moster|AI")
	TObjectPtr<UAIPerceptionComponent> AIPerception;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Moster|AI")
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Moster|AI")
	TObjectPtr<UAISenseConfig_Hearing> HearingConfig;
};
