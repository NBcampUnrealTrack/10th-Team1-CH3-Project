// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

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
	virtual void Tick(float DeltaTime) override;

	void Initialize();
	void SpawnRandomAI();
	APawn* SpawnAI();
	void RemoveSpawnedAIs();

  public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnVolume")
	TObjectPtr<USceneComponent> SceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnVolume")
	TObjectPtr<UBoxComponent> BoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnVolume")
	TArray<TObjectPtr<APawn>> SpawnedAIs;

  private:
	FOnPlayerEntered OnPlayerEndtered;
};
