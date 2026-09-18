// 26/09/14 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "Player/ActorComponent/StatComponent.h"

// UHT Header
#include "MonsterStatComponent.generated.h"

class UMonsterDataAsset;

UENUM(BlueprintType)
enum class EMonsterType : uint8
{
	Special UMETA(DisplayName = "Special"),
	Range UMETA(DisplayName = "Range"),
	Melee UMETA(DisplayName = "Melee"),
	Fly UMETA(DisplayName = "Fly"),
};

UCLASS()
class BEYONDOVERRIDE_API UMonsterStatComponent : public UStatComponent
{
	GENERATED_BODY()

	// Methtods
  public:
	UMonsterStatComponent();

	float GetWalkSpeed() const;

	float GetSprintSpeed() const;

	void SetAttackRange(float Range);
	float GetAttackRange() const;

	FVector GetAttackPoint() const;

	void SetMonsterID(FName ID);
	FName GetMonsterID() const;

	// Attack System
	void CallAttackLock();
	bool IsDelay() const;
	void Attack();
	void StatSetup();

	// Protect System
	void ApplyProtect(int32 getdamage, AActor* DamageCauser);

  protected:
	virtual void BeginPlay() override;
	void OnBalisticHit(AActor* Target);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Data")
	TObjectPtr<UMonsterDataAsset> MonsterData;

	// Properties
  protected:
	// Attack Info
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AttackDamage = 15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RapidCount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RapidDelay = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackDelay = 7.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BulletSpeed = 2500.0f;

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
	FTimerHandle AttackLock;

	// Monster key Info

	UPROPERTY(EditAnywhere, Category = "Moster|ID")
	FName MonsterID = "Gunner";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMonsterType MonsterType = EMonsterType::Range;
};
