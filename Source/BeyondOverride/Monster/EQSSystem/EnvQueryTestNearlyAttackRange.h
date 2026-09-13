// 26/09/10 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "EnvironmentQuery/EnvQueryTest.h"

// UHT Header
#include "EnvQueryTestNearlyAttackRange.generated.h"

UCLASS()
class BEYONDOVERRIDE_API UEnvQueryTestNearlyAttackRange : public UEnvQueryTest
{
	GENERATED_BODY()

  public:
	UEnvQueryTestNearlyAttackRange();

  protected:
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
};
