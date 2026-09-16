// 26/09/14 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "Components/ActorComponent.h"

// UHT Header
#include "ShortTermStateComponent.generated.h"

UENUM(BlueprintType)
enum class EFlag : uint8
{
	TakeDamage UMETA(DisplayName = "TakeDamage"),
	Calling UMETA(DisplayName = "Calling"),
	Hearing UMETA(DisplayName = "Hearing"),
};

USTRUCT()
struct FFlagInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	EFlag Flag;

	UPROPERTY(VisibleAnywhere)
	bool Complete = false;

	UPROPERTY(VisibleAnywhere)
	float CallTime;

	UPROPERTY(VisibleAnywhere)
	bool FlagType = false;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BEYONDOVERRIDE_API UShortTermStateComponent : public UActorComponent
{
	GENERATED_BODY()

	// Methtods
  public:
	UShortTermStateComponent();

	void PlantFlag(FFlagInfo FlagInfo);

	void PlantFlag(EFlag State, float Time);

	void PlantFlag(EFlag State, float Time, bool Type);

	bool FoldFlags(EFlag Target);

	bool FoldFlags(EFlag Target, bool& Type);

  protected:
	virtual void BeginPlay() override;

	// Flag Control
	void PopFlag();

	// Properties
  public:
  protected:
	UPROPERTY(VisibleAnywhere, Category = "State|Viewer")
	TArray<FFlagInfo> Flags;

	FTimerHandle FlagControlTimer;
};
