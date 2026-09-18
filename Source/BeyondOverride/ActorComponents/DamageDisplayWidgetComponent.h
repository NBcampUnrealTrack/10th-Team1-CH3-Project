#pragma once

#include "CoreMinimal.h"

#include "Components/WidgetComponent.h"

#include "DamageDisplayWidgetComponent.generated.h"

class UDamageDisplayWidget;

UCLASS(ClassGroup = (UI), meta = (BlueprintSpawnableComponent))
class BEYONDOVERRIDE_API UDamageDisplayWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

  protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UDamageDisplayWidget> DamageDisplayWidget;

  public:
	UDamageDisplayWidgetComponent();

  protected:
	virtual void BeginPlay() override;

  public:
	UFUNCTION(BlueprintCallable)
	void TakeDamage(int32 Damage);

  protected:
	// 누적 데미지 (적용 시간 내)
	int32 TotalDamage;

	// 데미지 누적 적용 시간
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DamageAccumulationDuration;
	// 데미지 누적 적용 타이머 핸들
	FTimerHandle DamageAccumulationTimerHandle;

  protected:
	// 데미지 누적 타이머 종료 시 호출되는 콜백 함수
	void OnDamageAccumulationEnded();
};
