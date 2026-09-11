// 26/09/10 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "Components/ActorComponent.h"

// UHT Header
#include "AttackDataComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BEYONDOVERRIDE_API UAttackDataComponent : public UActorComponent
{
	GENERATED_BODY()

  public:
	UAttackDataComponent();
	UFUNCTION()
	void SetTargetLocation(FVector Point);
	UFUNCTION()
	FVector GetTargetLocation() const;
	UFUNCTION()
	int32 GetAttackDamage() const;
	UFUNCTION()
	int32 GetRapidCount() const;
	UFUNCTION()
	float GetAttackSpeed() const;
	UFUNCTION()
	float GetAttackRange() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Monster")
	bool IsDelay() const;

	void EndAttackHold();

	void CallAttackDelay();

  protected:
	UPROPERTY(EditAnywhere, Category = "Monster|AttackData")
	int32 AttackDamage = 15;
	UPROPERTY(EditAnywhere, Category = "Monster|AttackData")
	int32 RapidCount = 3;
	UPROPERTY(EditAnywhere, Category = "Monster|AttackData")
	float AttackSpeed = 7.0f;
	UPROPERTY(EditAnywhere, Category = "Monster|AttackData")
	float AttackRange = 600.0f;
	UPROPERTY(EditAnywhere, Category = "Monster|AttackData")
	FVector TargetLocation;

	bool AttackHold = false;
	FTimerHandle AttackDelayHandler;

	virtual void BeginPlay() override;
};
