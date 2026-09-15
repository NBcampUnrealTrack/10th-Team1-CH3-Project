#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "ProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class AProjectileBase : public AActor
{
	GENERATED_BODY()

  protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> SceneRoot; // 루트 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement; // 탄도학 적용 컴포넌트

	int32 Damage; // 데미지

  public:
	AProjectileBase();

	virtual void Initialize(
		APawn* InInstigator,
		const int32 InDamage,
		const FVector& Velocity,
		const float GravityScale = 1.f,
		const float LifeSpan = 0.f);
};
