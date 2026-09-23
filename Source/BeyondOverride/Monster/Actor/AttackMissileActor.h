// 26/09/23 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "GameFramework/Actor.h"

// UHT include
#include "AttackMissileActor.generated.h"

class UCapsuleComponent;

UCLASS()
class BEYONDOVERRIDE_API AAttackMissileActor : public AActor
{
	GENERATED_BODY()

	// Methtods
  public:
	AAttackMissileActor();

	virtual float TakeDamage(float DamageAmount,
							 FDamageEvent const& DamageEvent,
							 AController* EventInstigator,
							 AActor* DamageCauser) override;

	void SetDamage(int32 Damage);

  protected:
	virtual void BeginPlay() override;

	void TargetPoint(FVector Point);

	void ExplosionSequnce();

	UFUNCTION()
	void OnCollisionHit(UPrimitiveComponent* HitComponent,
						AActor* OtherActor,
						UPrimitiveComponent* OtherComp,
						FVector NormalImpulse,
						const FHitResult& Hit);
	// Properties
  public:
  protected:
	uint32 BrokenCount = 0;

	uint32 ThisDamage = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack|AttackOwner")
	ACharacter* AttackOwner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack|Component")
	USceneComponent* RootComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack|Component")
	UCapsuleComponent* CollisionComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack|Component")
	UStaticMeshComponent* StaticMeshComp;

	// Effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Effect")
	TObjectPtr<UParticleSystem> ExplosionEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Effect")
	TObjectPtr<USoundBase> ExplosionSound;
};
