// 26/09/14 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "Player/ActorComponent/StatComponent.h"

// Add include
#include "Monster/Enums/InfoEnums.h"
#include "Monster/Enums/StateEnums.h"

// UHT Header
#include "MonsterStatComponent.generated.h"

class UMonsterDataAsset;
class BOCharacter;

UCLASS()
class BEYONDOVERRIDE_API UMonsterStatComponent : public UStatComponent
{
	GENERATED_BODY()

	// Methtods
  public:
	// Life Cycle Function
	UMonsterStatComponent();

	// Getter
	EMonsterType GetMonsterType() const;
	FVector GetAttackPoint() const;
	int32 GetAttackDamage() const;
	float GetSprintSpeed() const;
	float GetAttackRange() const;
	float GetWalkSpeed() const;
	FName GetMonsterID() const;
	float GetFlyMax() const;
	float GetFlyMin() const;

	// Setter
	void SetAttackRange(float Range);
	void SetMonsterID(FName ID);

	// Basic Stat System
	void StatSetup();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MonsterStat")
	int NowHP();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MonsterStat")
	int MaxHP();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MonsterStat")
	bool IsDead();

	// Attack System
	void CallAttackLock();
	bool IsDelay() const;

	void Attack();
	void BalisticFire();
	void OnMissileHit(TArray<FOverlapResult> Targets);
	void DamageLogic(AActor* Target, int32 Damage);

	// Protect System
	void ApplyProtect(int32 GetDamage, AActor* DamageCauser);

  protected:
	// Life Cycle Function
	virtual void BeginPlay() override;

	// Attack System
	void OnBalisticHit(AActor* Target);

	// Properties
  public:
  protected:
	// Attack Info
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AttackDamage = 15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RapidCount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurRapid = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RapidDelay = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackDelay = 7.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BulletSpeed = 2500.0f;

	FTimerHandle AttackLock;

	FTimerHandle RapidTimer;

	// Another Info
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Protect = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Intelligence = FMath::RandRange(0, 3);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SprintSpeed = 1.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FlyMin = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FlyMax = 500.0f;

	// Monster key Info

	UPROPERTY(EditAnywhere, Category = "Moster|ID")
	FName MonsterID = "Gunner";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMonsterType MonsterType = EMonsterType::Range;
};
