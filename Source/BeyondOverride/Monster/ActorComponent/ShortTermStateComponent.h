// 26/09/14 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "Components/ActorComponent.h"

// Add include
#include "Monster/Enums/InfoEnums.h"
#include "Monster/Enums/StateEnums.h"
#include "Monster/Structs/StateParams.h"

// UHT Header
#include "ShortTermStateComponent.generated.h"

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
