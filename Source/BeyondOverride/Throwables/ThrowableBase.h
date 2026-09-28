#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "ThrowableBase.generated.h"

UCLASS()
class BEYONDOVERRIDE_API AThrowableBase : public AActor
{
	GENERATED_BODY()

  protected:
	// Skeletal Mesh 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	// 활성화 파티클
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TObjectPtr<UParticleSystem> ActivationParticle;
	// 활성화 사운드
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TObjectPtr<USoundCue> ActivationSound;

	// 던질 때, Instigator와 충돌을 잠시 무시하기 위한 타이머 핸들
	FTimerHandle IgnoreInstigatorCollisionTimerHandle;
	// Instigator와의 충돌을 무시할 시간
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float IgnoreInstigatorCollisionTime;

  public:
	AThrowableBase();

	// 투척 액터 던지기
	virtual void Throw(
		APawn* InInstigator,
		const FRotator& Rotation,
		const float Force);

  protected:
	// 활성화
	virtual void Activate();

	// 활성화 이펙트 재생
	void PlayActivationEffects(
		const FVector& Location,
		const FRotator& Rotation);
};
