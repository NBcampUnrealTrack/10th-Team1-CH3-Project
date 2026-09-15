#pragma once

#include "CoreMinimal.h"

#include "Projectiles/Throwables/ThrowableProjectile.h"

#include "ExplosiveProjectile.generated.h"

UCLASS()
class BEYONDOVERRIDE_API AExplosiveProjectile : public AThrowableProjectile
{
	GENERATED_BODY()

  protected:
	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
	TObjectPtr<UParticleSystem> ExplosionParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
	TObjectPtr<USoundBase> ExplosionSound;

  public:
	AExplosiveProjectile();

  protected:
	virtual void Activate() override; // 딜레이 후 호출되어 작동
};
