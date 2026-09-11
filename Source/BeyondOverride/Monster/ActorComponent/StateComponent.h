// 26/09/10 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "Components/ActorComponent.h"

// UHT Header
#include "StateComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BEYONDOVERRIDE_API UStateComponent : public UActorComponent
{
	GENERATED_BODY()

  public:
	UStateComponent();

	FVector GetSpawnPoint() const;

	bool GetBeCanPatrol() const;

	bool GetContinueTargeting() const;

	void TrueBeCanPatrol();

	void FalseBeCanPatrol();

	void TrueContinueTargeting();

	void FalseContinueTargeting();

	void CallPatrolTimer();

	void CallContinueTimer();

	void ReCallContinueTimer();

  protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "State|AI Flag")
	FVector SpawnPoint;

	UPROPERTY(VisibleAnywhere, Category = "State|AI Flag")
	bool bCanPatrol = false;

	UPROPERTY(VisibleAnywhere, Category = "State|AI Flag")
	bool ContinueTargeting = true;

	FTimerHandle PatrolTimer;

	FTimerHandle ContinueTimer;
};
