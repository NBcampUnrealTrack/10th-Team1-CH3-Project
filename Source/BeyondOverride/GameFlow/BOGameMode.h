// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Enums/BOEnums.h"
#include "GameFramework/GameMode.h"
#include "Monster/Enums/InfoEnums.h"

#include "BOGameMode.generated.h"

class UBOGameInstance;
class UFarmingStateMachine;
class UDefenseStateMachine;

UCLASS()
class BEYONDOVERRIDE_API ABOGameMode : public AGameMode
{
	GENERATED_BODY()

  public:
	ABOGameMode();

	virtual void BeginPlay() override;

	void InitSetting();
	void StartGame(); // after start button clicked
	void EnterBunker();
	void ProvideBasicEquipment();
	void StartFarming(); // when interacting with the bunker entrance
	void EndFarming(EStageResult Result);
	void Die();
	void EnterAIBuilding();
	void ExitAIBuilding();
	void StartDefense(); // when interacting with the main computer first time
	void EndDefense();   // when end of defense;
	void ClearGame();    // when interacting with the main computer after defense
	void ShowEnding();   // after final result widget's ok button clicked
	void EndGame();      // after end of ending credit
	void ExitGame();     // after quit button clicked

	void AddKilledMonster(FName MonsterId, EMonsterType MonsterType);
	void SetKillerMonster(FName MonsterId);

	void GetKilledMonsters(TMap<FName, int32>& Data) const;
	FName GetKillerMonster() const;

  private:
	UPROPERTY()
	TObjectPtr<UBOGameInstance> GameInstance;

	UPROPERTY()
	TObjectPtr<UFarmingStateMachine> FarmingStateMachine;
	UPROPERTY()
	TObjectPtr<UDefenseStateMachine> DefenseStateMachine;

  private:
	TMap<FName, int32> KilledMonsters;
	FName KillerMonster;

	FTimerHandle DefenseTimer;
};
