// 26/09/10 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "Components/ActorComponent.h"

// UHT Header
#include "StateComponent.generated.h"

class UAIPerceptionComponent;
class ABOCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BEYONDOVERRIDE_API UStateComponent : public UActorComponent
{
	GENERATED_BODY()

  public:
	UStateComponent();

	void SetIsCallLocation(bool value);
	bool GetIsCallLocation() const;

	void SetTarget(ABOCharacter* Character);

	ABOCharacter* GetTarget() const;

	void SetLocationPatrolActor(AActor* PlayActor);

	AActor* GetLocationPatrolActor() const;

	void SetLocationPatrolPoint(FVector Location);

	FVector GetLocationPatrolPoint() const;

	FVector GetSpawnPoint() const;

	bool GetBeCanPatrol() const;

	void TrueBeCanPatrol();
	void FalseBeCanPatrol();

	void TrueContinueTargeting();
	void FalseContinueTargeting();

	void CallPatrolTimer();

	bool GetContinueTargeting() const;

	void CallContinueTimer();
	void ReCallContinueTimer();

	bool IsLocation() const;
	void CallLocationPatrolTimer();

	bool IsHearing() const;
	void CallHearingTimer();

	bool IsGetDamage() const;
	void CallGetDamage();

	bool IsCalling() const;
	void SetCallingTimer();

	bool IsStandOff() const;
	void SetStandOffTimer();

	bool IsContinueStandOff() const;
	void SetContinueStandOffTimer();

  protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "State|AI Flag")
	TObjectPtr<ABOCharacter> NowTarget;

	UPROPERTY(VisibleAnywhere, Category = "State|AI Flag")
	TObjectPtr<AActor> LocationPatrolActor;

	UPROPERTY(VisibleAnywhere, Category = "State|AI Flag")
	FVector LocationPatrolPoint;

	UPROPERTY(VisibleAnywhere, Category = "State|AI Flag")
	FVector SpawnPoint;

	UPROPERTY(VisibleAnywhere, Category = "State|AI Flag")
	bool bCanPatrol = false;

	UPROPERTY(VisibleAnywhere, Category = "State|AI Flag")
	bool ContinueTargeting = true;

	UPROPERTY(VisibleAnywhere, Category = "State|AI Flag")
	bool IsCallLocation = false;

	FTimerHandle GetDamageTimer;

	FTimerHandle HearingTimer;

	FTimerHandle PatrolTimer;

	FTimerHandle ContinueTimer;

	FTimerHandle LocationPatrolTimer;

	FTimerHandle CallingTimer;

	FTimerHandle StandOffTimer;

	FTimerHandle ContinueStandOffTimer;
};
