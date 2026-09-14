// 26/09/13 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "EnvironmentQuery/EnvQueryGenerator.h"

// UHT Header
#include "AIEnvQueryGenerator.generated.h"

UCLASS()
class BEYONDOVERRIDE_API UAIEnvQueryGenerator : public UEnvQueryGenerator
{
	GENERATED_BODY()

  public:
	UAIEnvQueryGenerator(const FObjectInitializer& ObjectInitializer);

  protected:
	virtual void GenerateItems(FEnvQueryInstance& QueryInstance) const override;
};
