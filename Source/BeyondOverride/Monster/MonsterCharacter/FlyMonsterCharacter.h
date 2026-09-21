// 26/09/09 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "GameFramework/Character.h"

// UHT Header
#include "FlyMonsterCharacter.generated.h"

UCLASS()
class BEYONDOVERRIDE_API AFlyMonsterCharacter : public ACharacter
{
	GENERATED_BODY()

  public:
	AFlyMonsterCharacter();

	virtual void Tick(float DeltaSecond) override;

  protected:
	virtual void BeginPlay() override;

	TArray<FVector> TestNav(const FVector& TargetLocation, const FVector& StartLocation);

	void MoveFlying(const FVector& TargetLocation);

	FVector GetWallEndPoint(const FVector& Start, const FVector& Direction, const FVector& WallLocation, const FVector& WallExtent, const FRotator& WallRotation);

  public:
	UPROPERTY(VisibleAnywhere, Category = "Monster|NavCheck")
	TArray<FVector> Paths;
};
