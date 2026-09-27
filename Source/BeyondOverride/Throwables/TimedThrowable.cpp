#include "Throwables/TimedThrowable.h"

ATimedThrowable::ATimedThrowable()
{
	ActivationDelay = 2.5f;
}

void ATimedThrowable::Throw(
	APawn* InInstigator,
	const FRotator& Rotation,
	const float Force)
{
	Super::Throw(InInstigator, Rotation, Force);

	// 던짐과 동시에 활성화 타이머 시작
	StartActivationTimer();
}

void ATimedThrowable::StartActivationTimer()
{
	GetWorldTimerManager().SetTimer(
		ActivationTimerHandle,
		this,
		&ATimedThrowable::Activate,
		ActivationDelay,
		false);
}
