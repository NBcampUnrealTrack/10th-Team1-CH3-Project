// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Enums/BOEnums.h"
#include "GameFramework/GameMode.h"

#include "BOGameMode.generated.h"

class UBOGameInstance;
class UFarmingStateMachine;

UCLASS()
class BEYONDOVERRIDE_API ABOGameMode : public AGameMode
{
	GENERATED_BODY()

  public:
	ABOGameMode();

	virtual void BeginPlay() override;

	void Start();
	void EnterBunker(EFarmingResult Result);
	void ProvideBasicEquipment();
	void StartFarming();
	void EndFarming(EFarmingResult Result);
	void ToEnding();
	void Explosion();
	void End();
	void Exit();

	void AddKilledMonster(FName MonsterId);
	void SetKillerMonster(FName MonsterId);

	void GetKilledMonsters(TMap<FName, int32>& Data) const;
	FName GetKillerMonster() const;

  public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameMode")
	TObjectPtr<UBOGameInstance> GameInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameMode")
	TObjectPtr<UFarmingStateMachine> StateMachine;

  private:
	TMap<FName, int32> KilledMonsters;
	FName KillerMonster;
};
