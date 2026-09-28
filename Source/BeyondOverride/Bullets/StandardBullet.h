#pragma once

#include "CoreMinimal.h"

#include "Bullets/BulletBase.h"

#include "StandardBullet.generated.h"

class USphereComponent;

UCLASS()
class BEYONDOVERRIDE_API AStandardBullet : public ABulletBase
{
	GENERATED_BODY()

  protected:
	// 총알 피격 이벤트
	virtual void HandleImpact(AActor* OtherActor, const FHitResult& Hit) override;
};
