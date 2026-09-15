// 26/09/14 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "Components/ActorComponent.h"

// UHT Header
#include "MeshDataComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BEYONDOVERRIDE_API UMeshDataComponent : public UActorComponent
{
	GENERATED_BODY()

  public:
	UMeshDataComponent();

	UParticleSystem* GetEffect() const;
	void SetEffect(UParticleSystem* Effect);

	FName GetAttackSocket() const;
	void SetAttackSocket(FName Socket);

	USkeletalMesh* GetSkeletal() const;
	void SetSkeletal(USkeletalMesh* Skeletal);

	UClass* GetAnim() const;
	void SetAnim(UClass* Animation);

	FName GetDefaultSkeletal() const;
	FName GetDefaultAnim() const;

  protected:
	virtual void BeginPlay() override;

	// Properties
  protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Effect")
	UParticleSystem* AttackEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Design")
	USkeletalMesh* TargetSkeletal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Design")
	UClass* TargetAnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Design")
	FName AttackSocket = "Muzzle_01";

	FName DefaultTargetSkeletal = "/Game/Assets/Monsters/ParagonWraith/Characters/Heroes/Wraith/Skins/ODGreen/Meshes/Wraith_ODGreen.Wraith_ODGreen";
	FName DefaultTargetAnimInstance = "/Game/Blueprints/Monster/Animation/ABP_Monster.ABP_Monster_C";
};
