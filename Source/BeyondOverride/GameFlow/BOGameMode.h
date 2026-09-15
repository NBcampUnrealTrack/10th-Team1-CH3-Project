// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Enums/BOEnums.h"
#include "GameFramework/GameMode.h"

#include "BOGameMode.generated.h"

class UFarmingStateMachine;

UCLASS()
class BEYONDOVERRIDE_API ABOGameMode : public AGameMode
{
	GENERATED_BODY()

  public:
	ABOGameMode();

	virtual void BeginPlay() override;

	void ProvideBasicEquipment();
	void StartFarming();
	void EndFarming(EFarmingResult Result);

	void GetKilledMonsters(TMap<FName, int32>& Data) const;
	FName GetKillerMonster() const;

  public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameMode")
	TObjectPtr<UFarmingStateMachine> StateMachine;

  private:
	TMap<FName, int32> KilledMonsters;
	FName KillerMonster;
};
