// 26/09/25 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "Monster/MonsterCharacter/MonsterCharacter.h"

// Add include
#include "Monster/Enums/MonsterValues.h"

// UHT Header
#include "BossCharacter.generated.h"

// DELEGATE
DECLARE_MULTICAST_DELEGATE(FHalfHealth);

UCLASS()
class BEYONDOVERRIDE_API ABossCharacter : public AMonsterCharacter
{
	GENERATED_BODY()

	// Methods
  public:
	ABossCharacter();

	virtual void MonsterAttack() override;

	virtual float TakeDamage(float DamageAmount,
							 FDamageEvent const& DamageEvent,
							 AController* EventInstigator,
							 AActor* DamageCauser) override;

	void MissileFire(bool IsRand);

	void MissileFire();

  protected:
	void ChangeMovement();

	void RecallSpawnPoint();

	void MissilePattern();

	// Properties
  public:
	FHalfHealth HalfHealth;

	int32 FireCount = 0;

  protected:
	FTimerHandle Recall;
	FTimerHandle UpTimer;
	FTimerHandle FireDelay;
};
