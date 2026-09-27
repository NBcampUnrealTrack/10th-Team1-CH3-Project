#pragma once

#include "CoreMinimal.h"

#include "Throwables/ThrowableBase.h"

#include "TimedThrowable.generated.h"

UCLASS()
class BEYONDOVERRIDE_API ATimedThrowable : public AThrowableBase
{
	GENERATED_BODY()

  protected:
	// 활성화되기까지 딜레이
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float ActivationDelay;
	// 활성화 타이머
	FTimerHandle ActivationTimerHandle;

  public:
	ATimedThrowable();

	// 투척 액터 던지기
	virtual void Throw(
		APawn* InInstigator,
		const FRotator& Rotation,
		const float Force) override;

  protected:
	// 활성화 타이머 시작
	void StartActivationTimer();
};
