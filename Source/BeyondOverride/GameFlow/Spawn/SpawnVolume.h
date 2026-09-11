// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../BODelegates.h"
#include "../Data/SpawnStruct.h"
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

	void SpawnAI();
	void SpawnRandomAI();

  public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnVolume")
	TObjectPtr<USceneComponent> SceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnVolume")
	TObjectPtr<UBoxComponent> BoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnVolume")
	FName Id;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnVolume")
	float SpawnExclusionRadius;

  public:
	FSpawnStruct SpawnVolumeData;
	FOnPlayerEntered OnPlayerEntered;
};
