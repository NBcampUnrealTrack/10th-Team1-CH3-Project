// 26/09/10 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "UObject/NoExportTypes.h"

// UHT Header
#include "MonsterCalling.generated.h"

class ABOCharacter;

UENUM(BlueprintType)
enum class ECallType : uint8
{
	LocationPatrol UMETA(DisplayName = "LocationPatrol"),
	Attack UMETA(DisplayName = "Attack"),
};

UCLASS()
class BEYONDOVERRIDE_API UMonsterCalling : public UObject
{
	GENERATED_BODY()
  public:
	void CallMonsters(const FVector& CallCenter, float Radius, ABOCharacter*& Target, ECallType Type);
};
