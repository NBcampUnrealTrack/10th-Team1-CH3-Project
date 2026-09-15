#pragma once

#include "CoreMinimal.h"

#include "Projectiles/ProjectileBase.h"

#include "ThrowableProjectile.generated.h"

class USphereComponent;

UCLASS()
class AThrowableProjectile : public AProjectileBase
{
	GENERATED_BODY()

  protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Collision;

	float Radius; // 적용 반경
	float Delay;  // 활성화 딜레이

	FTimerHandle ActivateTimerHandle; // 활성화 타이머 핸들

  public:
	AThrowableProjectile();

	virtual void Initalize(
		APawn* InInstigator,
		int32 InDamage,
		const float InRadius,
		const float InDelay,
		const FVector& Velocity,
		const float GravityScale);

  protected:
	virtual void Activate(); // 딜레이 후 호출되어 작동
};
