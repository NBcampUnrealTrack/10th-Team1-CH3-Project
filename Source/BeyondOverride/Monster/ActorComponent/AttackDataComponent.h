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

	// Setter
	UFUNCTION()
	void SetProtect(int32 GetProtect);
	UFUNCTION()
	void SetAttackDamage(int32 Damage);
	UFUNCTION()
	void SetRapidCount(int32 Rapid);
	UFUNCTION()
	void SetAttackDelay(float Speed);
	UFUNCTION()
	void SetAttackRange(float Range);
	UFUNCTION()
	void SetTargetLocation(FVector Point);

	// Getter
	UFUNCTION()
	int32 GetProtect() const;
	UFUNCTION()
	int32 GetAttackDamage() const;
	UFUNCTION()
	int32 GetRapidCount() const;
	UFUNCTION()
	float GetAttackDelay() const;
	UFUNCTION()
	float GetAttackRange() const;
	UFUNCTION()
	FVector GetTargetLocation() const;

	// Function
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Monster")
	bool IsDelay() const;

	void CallAttackDelay();

  protected:
	UPROPERTY(EditAnywhere, Category = "Monster|AttackData")
	int32 Protect = 15;
	UPROPERTY(EditAnywhere, Category = "Monster|AttackData")
	int32 AttackDamage = 15;
	UPROPERTY(EditAnywhere, Category = "Monster|AttackData")
	int32 RapidCount = 3;
	UPROPERTY(EditAnywhere, Category = "Monster|AttackData")
	float AttackDelay = 7.0f;
	UPROPERTY(EditAnywhere, Category = "Monster|AttackData")
	float AttackRange = 600.0f;
	UPROPERTY(EditAnywhere, Category = "Monster|AttackData")
	FVector TargetLocation;

	FTimerHandle AttackDelayHandler;

	virtual void BeginPlay() override;
};
