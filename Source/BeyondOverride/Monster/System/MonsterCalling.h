// 26/09/10 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "UObject/NoExportTypes.h"

// Add include
#include "Monster/Enums/SystemEnums.h"

// UHT Header
#include "MonsterCalling.generated.h"

class ABOCharacter;

UCLASS()
class BEYONDOVERRIDE_API UMonsterCalling : public UObject
{
	GENERATED_BODY()
  public:
	void CallMonsters(const FVector& CallCenter, float Radius, ABOCharacter*& Target, ECallType Type);
};
