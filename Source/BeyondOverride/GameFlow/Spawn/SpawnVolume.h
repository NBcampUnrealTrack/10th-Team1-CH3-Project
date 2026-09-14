// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/BoxComponent.h"
#include "DataTables/Farming/PhaseData.h"
#include "DataTables/Farming/SpawnData.h"
#include "GameFlow/BODelegates.h"
#include "GameFramework/Actor.h"
#include "Interaction/Actors/ExitActor.h"

#include "SpawnVolume.generated.h"

UCLASS()
class BEYONDOVERRIDE_API ASpawnVolume : public AActor
{
	GENERATED_BODY()

  public:
	ASpawnVolume();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "SpawnVolume")
	virtual void OnOverlapped(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	void SpawnMonsters();
	void SpawnRandomMonster(TArray<FSpawnEntry>& SpawnEntries, float MinDist = -1.0f, float MaxDist = -1.0f, bool IsChase = false);
	void StartPhase();
	void SpawnPhaseMonsters();

	FName GetId() const;
	FName GetRegionId() const;

  public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpawnVolume")
	TObjectPtr<USceneComponent> SceneComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpawnVolume")
	TObjectPtr<UBoxComponent> BoxComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpawnVolume")
	FName Id;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpawnVolume")
	float SpawnMinRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpawnVolume")
	float SpawnMaxRadius;

  private:
	int32 PhaseIndex;

	FSpawnData SpawnVolumeData;
	FPhaseData PhaseData;

	FTimerHandle PhaseTimer;

  public:
	FOnPlayerEntered OnPlayerEntered;
};
