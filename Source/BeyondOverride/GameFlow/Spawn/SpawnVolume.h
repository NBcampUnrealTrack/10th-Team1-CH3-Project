// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../../DataTables/Farming/PhaseData.h"
#include "../../DataTables/Farming/SpawnData.h"
#include "../../Interaction/Actors/ExitActor.h"
#include "../BODelegates.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"

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

	void SpawnMonster();
	void SpawnRandomMonster(TArray<FSpawnEntry>& SpawnEntries, float MinDist = -1.0f, float MaxDist = -1.0f, bool IsChase = false);
	void StartPhase();
	void SpawnPhaseMonster();

	FName GetId() const;
	FName GetRegionId() const;

  public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnVolume")
	TObjectPtr<USceneComponent> SceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnVolume")
	TObjectPtr<UBoxComponent> BoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnVolume")
	FName Id;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnVolume")
	float SpawnMinRadius;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnVolume")
	float SpawnMaxRadius;

  public:
	int32 PhaseIndex;

	FSpawnData SpawnVolumeData;
	FPhaseData PhaseData;

	FTimerHandle PhaseTimer;
	FOnPlayerEntered OnPlayerEntered;
};
