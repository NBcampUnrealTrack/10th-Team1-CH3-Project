#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "ProjectileBase.generated.h"

UCLASS()
class AProjectileBase : public AActor
{
	GENERATED_BODY()

  public:
	AProjectileBase();

  protected:
	virtual void BeginPlay() override;

  public:
	virtual void Tick(float DeltaTime) override;
};
