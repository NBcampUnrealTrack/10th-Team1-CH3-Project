#pragma once

#include "CoreMinimal.h"

#include "Bullets/BulletBase.h"

#include "ExplosiveBullet.generated.h"

class USphereComponent;

UCLASS()
class BEYONDOVERRIDE_API AExplosiveBullet : public ABulletBase
{
	GENERATED_BODY()

  protected:
	// 폭발 범위 콜리전
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> ExplosiveCollision;

	// 최소 데미지 배율
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MinDamageMultiplier;

  public:
	AExplosiveBullet();

  protected:
	// 총알 피격 이벤트
	virtual void HandleImpact(AActor* OtherActor, const FHitResult& Hit) override;
};
