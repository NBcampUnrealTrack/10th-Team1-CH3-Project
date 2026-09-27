#pragma once

#include "CoreMinimal.h"

#include "Throwables/ThrowableBase.h"

#include "ImpactThrowable.generated.h"

UCLASS()
class BEYONDOVERRIDE_API AImpactThrowable : public AThrowableBase
{
	GENERATED_BODY()

  public:
	AImpactThrowable();

  protected:
	// Hit 이벤트
	UFUNCTION()
	virtual void OnHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);
};
