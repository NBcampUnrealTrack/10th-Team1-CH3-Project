// 26/09/14 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "EnvironmentQuery/EnvQueryTest.h"

// UHT Header
#include "EnvQueryTestRand.generated.h"

UCLASS()
class BEYONDOVERRIDE_API UEnvQueryTestRand : public UEnvQueryTest
{
	GENERATED_BODY()

  public:
	UEnvQueryTestRand();

  protected:
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
};
