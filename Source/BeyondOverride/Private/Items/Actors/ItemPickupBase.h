#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "ItemPickupBase.generated.h"

UCLASS()
class AItemPickupBase : public AActor
{
	GENERATED_BODY()

  public:
	AItemPickupBase();

  protected:
	virtual void BeginPlay() override;

  public:
	virtual void Tick(float DeltaTime) override;
};
