// 26/09/23 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "Components/ActorComponent.h"

// Add include

// UHT Header
#include "AirNavComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BEYONDOVERRIDE_API UAirNavComponent : public UActorComponent
{
	GENERATED_BODY()

	// Methtods
  public:
	UAirNavComponent();

	TArray<FVector> AirNav(const FVector& TargetLocation,
						   const FVector& StartLocation);

  protected:
	virtual void BeginPlay() override;

	FVector GetWallEndPoint(FVector DirectionData,
							FVector ImpactData,
							FVector TargetOrigin,
							FVector TargetExtent,
							FRotator TargetRotation,
							float& Distance);

	// Properties
  public:
	UPROPERTY(VisibleAnywhere, Category = "Monster|NavCheck")
	TArray<FVector> Paths;
};
