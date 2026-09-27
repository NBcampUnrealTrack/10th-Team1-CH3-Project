// 26/09/25 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "Monster/AIController/MonsterAIController.h"

// UHT Header
#include "BossAIController.generated.h"

class ABossCharacter;
class UBossPhaseComponent;

UCLASS()
class BEYONDOVERRIDE_API ABossAIController : public AMonsterAIController
{
	GENERATED_BODY()
	// Methods
  public:
	ABossAIController();

	// Functions
	void SetPhase1();
	void SetPhase2();

	// Recalls
	void RecallStart();

	void RecallEnd();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "BossState")
	bool IsRecall();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "BossState")
	EBossPhase CurrentPhase() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "BossState")
	EBossPattern PatternCast() const;

	void PatternHold(EBossPattern HoldPattern);

	ABossCharacter* GetBoss() const;

  protected:
	virtual void OnPossess(APawn* InPawn) override;

	virtual void PostInitializeComponents() override;

	// Functions
	void StandOffBlocking(const EMonsterState& CastState);

	// Properties
  public:
	bool bRecall;

  protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|State")
	TObjectPtr<UBossPhaseComponent> BossPhase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|State")
	EBossPattern Pattern;

	FTimerHandle RecallTimer;
	FTimerHandle ChangeTimer;
};
