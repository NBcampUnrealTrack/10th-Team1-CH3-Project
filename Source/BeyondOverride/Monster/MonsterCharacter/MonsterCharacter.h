// 26/09/09 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "GameFramework/Character.h"

// UHT Header
#include "MonsterCharacter.generated.h"

// 전방 선언
class UStateComponent;
class UAttackDataComponent;
class UStatComponent;
class UMonsterStatComponent;
class UMonsterDataAsset;

UENUM(BlueprintType)
enum class EMonsterType : uint8
{
	Special UMETA(DisplayName = "Special"),
	Range UMETA(DisplayName = "Range"),
	Melee UMETA(DisplayName = "Melee"),
};

UCLASS()
class BEYONDOVERRIDE_API AMonsterCharacter : public ACharacter
{
	GENERATED_BODY()

  public:
	AMonsterCharacter();

	void SetMonsterID(FName ID);
	FName GetMonsterID() const;

	FVector GetAttackPoint() const;

	float GetAttackRange() const;

	bool IsDelay();

	void MonsterAttack();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Monster")
	UStateComponent* GetState() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Monster")
	UAttackDataComponent* GetAttackData() const;

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	void DeathSequence();

	UPROPERTY(EditAnywhere, Category = "Monster|Stat")
	float WalkSpeed = 600.0f;

	UPROPERTY(EditAnywhere, Category = "Monster|Stat")
	float SprintSpeed = 800.0f;

	UPROPERTY(EditAnywhere)
	EMonsterType MonsterType;

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

	UPROPERTY(VisibleAnywhere, Category = "Moster|Stat")
	TObjectPtr<UStatComponent> StatComponent;
	UPROPERTY(VisibleAnywhere, Category = "Coponent|State")
	TObjectPtr<UStateComponent> StateComponent;
	UPROPERTY(VisibleAnywhere, Category = "Coponent|Stat")
	TObjectPtr<UAttackDataComponent> AttackDataComponent;
};
