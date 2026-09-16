// 26/09/09 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "GameFramework/Character.h"

// UHT Header
#include "MonsterCharacter.generated.h"

// 전방 선언
class UMonsterStatComponent;
class UMonsterDataAsset;

UCLASS()
class BEYONDOVERRIDE_API AMonsterCharacter : public ACharacter
{
	GENERATED_BODY()

	// Methtods
  public:
	AMonsterCharacter();

	void SetMonsterID(FName ID);
	FName GetMonsterID() const;

	FVector GetAttackPoint() const;

	float GetAttackRange() const;

	bool IsDelay();

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	void MonsterAttack();

	void DeathSequence();

	UPROPERTY(EditAnywhere, Category = "Monster|Stat")
	float WalkSpeed = 600.0f;

	UPROPERTY(EditAnywhere, Category = "Monster|Stat")
	float SprintSpeed = 800.0f;

  protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	void SetUpMesh();

	// Properties
  public:
  protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Data")
	TObjectPtr<UMonsterDataAsset> MonsterData;
	UPROPERTY(VisibleAnywhere, Category = "Monster|Stat")
	TObjectPtr<UMonsterStatComponent> MonsterStat;

	FName SocketName;
	TObjectPtr<UParticleSystem> Effect;
};
