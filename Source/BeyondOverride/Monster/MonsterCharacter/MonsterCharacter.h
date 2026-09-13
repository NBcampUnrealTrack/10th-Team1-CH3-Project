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

struct FBallisticInfo
{
	bool bHit;

	uint32 EndCount = 0;

	float BulletSpeed = 2500.0f;
	FVector BulletLocation;
	FVector BulletDirection;

	FVector StartLocation = FVector::ZeroVector;
	FVector EndLocation = FVector::ZeroVector;

	float FlyTime = 0.1f;
	const FVector Gravity = FVector(0.0f, 0.0f, -980.0f);

	FHitResult HitResult;

	FTimerHandle Update;

	FCollisionQueryParams QueryParams;
	FCollisionObjectQueryParams TraceParams;
};

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

	void MonsterAttack();

	void CallBallistic();

	FBallisticInfo Ballistic;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Effect")
	UParticleSystem* FireParticle;

  protected:
	UPROPERTY(VisibleAnywhere, Category = "Coponent|Stat")
	TObjectPtr<UStatComponent> StatComponent;
	UPROPERTY(VisibleAnywhere, Category = "Coponent|State")
	TObjectPtr<UStateComponent> StateComponent;
	UPROPERTY(VisibleAnywhere, Category = "Coponent|Stat")
	TObjectPtr<UAttackDataComponent> AttackDataComponent;

	virtual void BeginPlay() override;

  public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
