// 26/09/13 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "EnvironmentQuery/EnvQueryTest.h"

// UHT Header
#include "EnvQueryTestHorizontal.generated.h"

UCLASS()
class BEYONDOVERRIDE_API UEnvQueryTestHorizontal : public UEnvQueryTest
{
	GENERATED_BODY()

  public:
	UEnvQueryTestHorizontal();

  protected:
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
};
