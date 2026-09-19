// 26/09/19 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "Kismet/BlueprintFunctionLibrary.h"

// UHT Header
#include "BFLMeleeAttack.generated.h"

UCLASS()
class BEYONDOVERRIDE_API UBFLMeleeAttack : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
  public:
	UFUNCTION(BlueprintCallable, Category = "Melee Attack")
	static AActor* DashAttack(ACharacter* Caster, float AttackRange);

  private:
	static AActor* DashAttack(ACharacter* Caster, float AttackRange, TArray<AActor*> Ignores, FVector RecallSPoint = FVector::ZeroVector, FVector RecallEPoint = FVector::ZeroVector);
};
