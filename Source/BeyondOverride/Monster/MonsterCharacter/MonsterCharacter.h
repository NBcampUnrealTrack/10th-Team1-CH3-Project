// 26/09/09 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "GameFramework/Character.h"

// Add Include
#include "Monster/ActorComponent/MonsterStatComponent.h"

// UHT Header
#include "MonsterCharacter.generated.h"

// DELEGATE
DECLARE_MULTICAST_DELEGATE(FOnStatSetComplete);
DECLARE_MULTICAST_DELEGATE(FOnDeleteMonster);

// 전방 선언
class UMonsterStatComponent;
class UMonsterDataAsset;

UCLASS()
class BEYONDOVERRIDE_API AMonsterCharacter : public ACharacter
{
	GENERATED_BODY()

	// Methods
  public:
	AMonsterCharacter();

	// MoveInput
	void MoveFlying(const FVector& TargetLocation);

	// Setters
	void SetMonsterID(FName ID);

	// Getters
	FName GetMonsterID() const;
	UMonsterDataAsset* GetMonsterData() const;
	float GetFlyMax() const;
	float GetFlyMin() const;
	FVector GetAttackPoint() const;
	FRotator GetAttackRotator() const;
	float GetAttackRange() const;

	UFUNCTION(BlueprintCallable, Category = "MonsterID")
	EMonsterType GetMonsterType() const;

	UMonsterStatComponent* GetMonsterStats() const;

	// Functions
	void OnMissileHit(TArray<FOverlapResult> Targets);

	void FocusSetUp(bool data);

	bool IsDelay();

	virtual void MonsterAttack();

	void DeathSequence(bool Cast = false);

	void EraseMonster();

	virtual float TakeDamage(float DamageAmount,
							 FDamageEvent const& DamageEvent,
							 AController* EventInstigator,
							 AActor* DamageCauser) override;

  protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	void SetUpMesh();

	// Properties
  public:
	FOnStatSetComplete OnStatSetComplete;
	FOnDeleteMonster OnDeleteMonster;

  protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Data")
	TObjectPtr<UMonsterDataAsset> MonsterData;

	UPROPERTY(VisibleAnywhere, Category = "Monster|Stat")
	TObjectPtr<UMonsterStatComponent> MonsterStat;

	FName SocketName;
	TObjectPtr<UParticleSystem> Effect;

	FTimerHandle DeathMotionTimer;
};
