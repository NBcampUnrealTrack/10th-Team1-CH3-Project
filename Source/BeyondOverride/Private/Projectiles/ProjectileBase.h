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
	TObjectPtr<USceneComponent> SceneRoot;                        // 루트 컴포넌트
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;  // 탄도학 적용 컴포넌트

  public:
	AProjectileBase();
};
