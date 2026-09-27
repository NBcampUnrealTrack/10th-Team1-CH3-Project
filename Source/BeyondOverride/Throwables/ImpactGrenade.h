#pragma once

#include "CoreMinimal.h"

#include "Throwables/ImpactThrowable.h"

#include "ImpactGrenade.generated.h"

class USphereComponent;

UCLASS()
class BEYONDOVERRIDE_API AImpactGrenade : public AImpactThrowable
{
	GENERATED_BODY()

  protected:
	// 폭발 범위 콜리전
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> ExplosiveCollision;

	// 최소 데미지 배율
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MinDamageMultiplier;

	// 기본 데미지
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats", meta = (ClampMin = "0", UIMin = "0"))
	int32 BaseDamage;

  public:
	AImpactGrenade();

  protected:
	virtual void Activate() override;
};
