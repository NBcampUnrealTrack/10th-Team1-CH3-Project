// 26/09/26 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "Components/ActorComponent.h"

// Add include
#include "Monster/Enums/StateEnums.h"

// UHT Header
#include "BossPhaseComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BEYONDOVERRIDE_API UBossPhaseComponent : public UActorComponent
{
	GENERATED_BODY()

  public:
	// Sets default values for this component's properties
	UBossPhaseComponent();

	// Functions
	void SetPhase1();
	void SetPhase2();

	EBossPhase CurrentPhase() const;

  protected:
	// Properties
  public:
  protected:
	UPROPERTY(VisibleAnywhere, Category = "Boss|Phase")
	EBossPhase Phase;
};
