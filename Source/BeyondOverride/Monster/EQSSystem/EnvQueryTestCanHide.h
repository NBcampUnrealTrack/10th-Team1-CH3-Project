// 26/09/13 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "EnvironmentQuery/EnvQueryTest.h"

// UHT Header
#include "EnvQueryTestCanHide.generated.h"

UCLASS()
class BEYONDOVERRIDE_API UEnvQueryTestCanHide : public UEnvQueryTest
{
	GENERATED_BODY()

  public:
	UEnvQueryTestCanHide();

  protected:
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
};
