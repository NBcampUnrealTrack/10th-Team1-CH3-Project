// 26/09/28 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "Interaction/Actors/StorageContainerActor.h"

// UHT Header
#include "MonsterStorageContainerActor.generated.h"

class USkeletalMeshComponent;

UCLASS()
class BEYONDOVERRIDE_API AMonsterStorageContainerActor : public AStorageContainerActor
{
	GENERATED_BODY()

	// Methtods
  public:
	AMonsterStorageContainerActor();

	void MeshInfoSetUp(FName ID,
					   FVector Scale,
					   FVector Location,
					   FRotator Rotation,
					   USkeletalMesh* Skeletal,
					   TSubclassOf<UAnimInstance> Anim);

  protected:
	// Life Cycle Function
	virtual void PostInitializeComponents() override;

	void EraseContainer();
	// Properties
  public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<USkeletalMeshComponent> SkeletalComp;

	bool bInfoSetUpComplete = false;

  protected:
	FName StorageID;
	FVector SkeletalScale = FVector(10.0f, 10.0f, 10.0f);
	FVector SkeletalLocation = FVector(0.0f, 0.0f, 0.0f);
	FRotator SkeletalRotation = FRotator(0.0f, 0.0f, 0.0f);
	TObjectPtr<USkeletalMesh> StorageSkeletal;
	TSubclassOf<UAnimInstance> StorageAnimInstance;

	FTimerHandle DeleteContainer;
};
