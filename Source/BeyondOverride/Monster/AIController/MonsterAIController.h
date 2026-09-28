// 26/09/09 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "AIController.h"

// Add include
#include "Monster/Enums/InfoEnums.h"
#include "Monster/Enums/MonsterValues.h"
#include "Monster/Enums/StateEnums.h"
#include "Monster/Structs/StateParams.h"
#include "Monster/Structs/SystemParams.h"
#include "Perception/AIPerceptionTypes.h"

// UHT Header
#include "MonsterAIController.generated.h"

// 전방 선언
class UMonsterStatComponent;
class UMonsterDataAsset;
class AMonsterCharacter;
class UAirNavComponent;
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

	// MonsterCharacter

	EPatrolType IsPatrolType() const;

	UFUNCTION(BlueprintCallable, Category = "Monster|Stat")
	UMonsterStatComponent* GetMonsterStats() const;

	UMonsterDataAsset* GetMonsterData() const;

	AMonsterCharacter* GetMonster() const;

	// Event

	void PlantFlag(FFlagInfo FlagInfo);

	void PlantFlag(EFlag State, float Time);

	void PlantFlag(EFlag State, float Time, bool Type);

	bool FoldFlags(EFlag Target);

	bool FoldFlags(EFlag Target, bool& Type);

	// Continuous State

	void OnStandOff();

	bool StandOffGetPosition() const;

	void StateChange(EMonsterState Input);

	void StateChange(EMonsterState Input, float HoldTime);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MonsterState")
	EMonsterState GetState() const;

	EMonsterState GetBeforeState() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MonsterState")
	bool IsContinueState() const;

	// Sense

	void SetTarget(ABOCharacter* Target);
	ABOCharacter* GetTarget() const;

	void SetTargetPoint(FVector Point);
	FVector GetTargetPoint() const;

	void SetEQSPoint(FVector Point);
	FVector GetEQSPoint() const;

	FVector GetSpawnPoint() const;

	bool AirNavControl(FVector TargetLocation);

	FVector AirNavResult();

	bool PathControl();

	void FocusSetUp(const EMonsterState& input);

	EPointPatrolState NowPatrolState() const;
	void ChangePatrolState();

	int GetPointX() const;
	int GetPointY() const;

	// MoveInput
	void MoveFlying(const FVector& TargetLocation);

  protected:
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnTargetHearUpdated(AActor* Actor, FAIStimulus Stimulus);

	void StopTree();

	// AI Controller가 Pawn 조종 시작시의 함수, override를 통해 재정의
	virtual void OnPossess(APawn* InPawn) override;

	// Properties
  public:
  protected:
	// MonsterData

	// BT
	UPROPERTY(EditDefaultsOnly, Category = "AI|Control")
	class UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Control")
	class UBehaviorTree* AirBehaviorTreeAsset;

	// Add Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|State")
	TObjectPtr<UAirNavComponent> AirNav;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|State")
	TObjectPtr<UContinuousStateComponent> State;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|State")
	TObjectPtr<UShortTermStateComponent> Flag;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|State")
	TObjectPtr<USenseComponent> SenseValue;

	// Perceptions
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Moster|AI")
	TObjectPtr<UAIPerceptionComponent> AIPerception;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Moster|AI")
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Moster|AI")
	TObjectPtr<UAISenseConfig_Hearing> HearingConfig;
};
