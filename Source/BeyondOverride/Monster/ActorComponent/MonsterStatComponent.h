// 26/09/14 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "Player/ActorComponent/StatComponent.h"

// UHT Header
#include "MonsterStatComponent.generated.h"

UENUM(BlueprintType)
enum class EMonsterType : uint8
{
	Special UMETA(DisplayName = "Special"),
	Range UMETA(DisplayName = "Range"),
	Melee UMETA(DisplayName = "Melee"),
};

UCLASS()
class BEYONDOVERRIDE_API UMonsterStatComponent : public UStatComponent
{
	GENERATED_BODY()

	// Methtods
  public:
	UMonsterStatComponent();

	void SetAttackRange(float Range);
	float GetAttackRange() const;

	FVector GetAttackPoint() const;

	void SetMonsterID(FName ID);
	FName GetMonsterID() const;

	// Attack System
	void CallAttackLock();
	bool IsDelay() const;
	void Attack();

	// Protect System
	void ApplyProtect(int32 getdamage, AActor* DamageCauser);

  protected:
	virtual void BeginPlay() override;
	void OnBalisticHit();

	// Properties
  protected:
	// Attack Info
	int32 AttackDamage = 15;

	int32 RapidCount = 3;

	float RapidDelay = 0.05f;

	float AttackDelay = 7.0f;

	float AttackRange = 600.0f;

	float BulletSpeed = 2500.0f;

	// Another Info
	int32 Protect = 5;

	float WalkSpeed;

	FHitResult RangeAttackResult;

	FTimerHandle AttackLock;

	// Monster key Info

	UPROPERTY(VisibleAnywhere, Category = "Moster|ID")
	FName MonsterID = "Wraith";

	EMonsterType MonsterType;
};
