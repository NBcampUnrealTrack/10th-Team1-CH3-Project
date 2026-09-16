// 26/09/14 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "Components/ActorComponent.h"

// UHT Header
#include "ContinuousStateComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnStateCast, const EMonsterState&);

UENUM(BlueprintType)
enum class EMonsterState : uint8
{
	Chase UMETA(DisplayName = "Chase"),
	Patrol UMETA(DisplayName = "Patrol"),
	Attack UMETA(DisplayName = "Attack"),
	StandOff UMETA(DisplayName = "StandOff"),
	Atmosphere UMETA(DisplayName = "Atmosphere"),
	LocationPatrol UMETA(DisplayName = "LocationPatrol"),
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BEYONDOVERRIDE_API UContinuousStateComponent : public UActorComponent
{
	GENERATED_BODY()

	// Methtods
  public:
	UContinuousStateComponent();

	bool IsContinueState() const;

	void StateChange(EMonsterState Input);

	void StateChange(EMonsterState Input, float HoldTime);

	EMonsterState GetState() const;

  protected:
	void StateAutoControl();

	virtual void BeginPlay() override;

	// Properties
  public:
	FOnStateCast OnStateCast;

  protected:
	UPROPERTY(VisibleAnywhere, Category = "State|Viewer")
	EMonsterState NowState;

	FTimerHandle StateTimer;

	FTimerHandle StandOffTimer;
};
