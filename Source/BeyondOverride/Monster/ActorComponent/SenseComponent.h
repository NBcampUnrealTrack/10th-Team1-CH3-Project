// 26/09/15 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "Components/ActorComponent.h"

// UHT Header
#include "SenseComponent.generated.h"

class ABOCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BEYONDOVERRIDE_API USenseComponent : public UActorComponent
{
	GENERATED_BODY()

  public:
	USenseComponent();

	void SenseSetup();

	void SetTarget(ABOCharacter* Target);
	ABOCharacter* GetTarget() const;

	void SetTargetPoint(FVector Point);
	FVector GetTargetPoint() const;

	void SetSpawnPoint(FVector Point);
	FVector GetSpawnPoint() const;

	float GetMemorize() const;
	float GetHearSenseSize() const;
	float GetLoseSightSize() const;
	float GetSightSenseSize() const;
	float GetVisionAngleDegrees() const;

  protected:
	virtual void BeginPlay() override;

	// Properties
  public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Data")
	TObjectPtr<UMonsterDataAsset> MonsterData;

  protected:
	UPROPERTY(VisibleAnywhere, Category = "State|SenseValue")
	TObjectPtr<ABOCharacter> MonsterTarget;

	UPROPERTY(VisibleAnywhere, Category = "State|SenseValue")
	FVector TargetPoint = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, Category = "State|SenseValue")
	FVector SpawnPoint;

	UPROPERTY(VisibleAnywhere, Category = "State|SenseValue")
	float HearSenseSize = 1750.0f;

	UPROPERTY(VisibleAnywhere, Category = "State|SenseValue")
	float SightSenseSize = 2500.0f;

	UPROPERTY(VisibleAnywhere, Category = "State|SenseValue")
	float LoseSightSize = 3000.0f;

	UPROPERTY(VisibleAnywhere, Category = "State|SenseValue")
	float VisionAngleDegrees = 50.0f;

	UPROPERTY(VisibleAnywhere, Category = "State|SenseValue")
	float Memorize = 5.0f;
};
