// 26/09/23 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "Components/ActorComponent.h"

// Add include
#include "Monster/Enums/InfoEnums.h"
#include "Monster/Structs/SystemParams.h"

// UHT Header
#include "AirNavComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BEYONDOVERRIDE_API UAirNavComponent : public UActorComponent
{
	GENERATED_BODY()

	// Methtods
  public:
	UAirNavComponent();

	bool AirNavControl(FVector TargetLocation);

	bool PathControl();

	FVector AirNavResult();

  protected:
	virtual void BeginPlay() override;

	void NavMakePaths();

	TArray<FVector> AirNav(const FVector& TargetLocation,
						   const FVector& StartLocation);

	FVector GetWallEndPoint(FVector DirectionData,
							FVector ImpactData,
							FVector TargetOrigin,
							FVector TargetExtent,
							FRotator TargetRotation,
							const FTransform& WallTransform,
							float& Distance);

	// Properties
  public:
	// NavControl
	UPROPERTY(VisibleAnywhere, Category = "Monster|AirNav")
	bool AirNavOn = false;

	UPROPERTY(VisibleAnywhere, Category = "Monster|AirNav")
	EAirNavState AirNavState = EAirNavState::Continue;

	// Sample Data
	UPROPERTY(VisibleAnywhere, Category = "Monster|AirNav")
	int32 SamplingPlayCount = 0;

	// NavData
	UPROPERTY(VisibleAnywhere, Category = "Monster|AirNav")
	FVector NavStartLocation = FVector::ZeroVector;
	UPROPERTY(VisibleAnywhere, Category = "Monster|AirNav")
	FVector NavEndLocation = FVector::ZeroVector;
	UPROPERTY(VisibleAnywhere, Category = "Monster|AirNav")
	TArray<FVector> Paths;
};
