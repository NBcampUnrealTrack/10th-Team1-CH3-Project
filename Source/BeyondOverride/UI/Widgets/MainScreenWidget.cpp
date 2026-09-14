#include "MainScreenWidget.h"

#include "Components/PanelWidget.h"
#include "Components/ProgressBar.h"
#include "Player/ActorComponent/StatComponent.h"
#include "Player/Character/BOCharacter.h"

void UMainScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OwningCharacter = Cast<ABOCharacter>(GetOwningPlayerPawn());
	if (!OwningCharacter)
		return;

	StatComponent = OwningCharacter->GetStatComponent();
	if (!StatComponent)
		return;

	StatComponent->OnHealthChanged.AddDynamic(this, &UMainScreenWidget::HandleHealthChanged);
	StatComponent->OnShieldChanged.AddDynamic(this, &UMainScreenWidget::HandleShieldChanged);

	HandleHealthChanged(StatComponent->GetCurHealth(), StatComponent->GetMaxHealth());
	HandleShieldChanged(StatComponent->GetCurShield(), StatComponent->GetMaxShield());
}

void UMainScreenWidget::NativeDestruct()
{
	if (StatComponent)
	{
		StatComponent->OnHealthChanged.RemoveDynamic(this, &UMainScreenWidget::HandleHealthChanged);
		StatComponent->OnShieldChanged.RemoveDynamic(this, &UMainScreenWidget::HandleShieldChanged);
		StatComponent = nullptr;
	}

	Super::NativeDestruct();
}

void UMainScreenWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateCompass();
}

void UMainScreenWidget::UpdateCompass()
{
	if (!CompassTick || !OwningCharacter)
		return;

	const AController* Controller = OwningCharacter->GetController();
	if (!Controller)
		return;

	// 0~360도로 정규화
	float Yaw = Controller->GetControlRotation().Yaw + CompassYawOffset;
	Yaw = FMath::Fmod(Yaw, 360.0f);
	if (Yaw < 0.0f)
		Yaw += 360.0f;

	const float PixelsPerDegree = TickUnitWidth / DegreesPerTick;
	const float OneLoopWidth = PixelsPerDegree * 360.0f;

	const float PointerScreenX = CompassViewWidth * 0.5f;

	// 가운데(2번째) 바퀴 시작점이 포인터 위치에 오도록 이동량 계산
	const float TranslationX = PointerScreenX - OneLoopWidth - (Yaw * PixelsPerDegree);

	CompassTick->SetRenderTranslation(FVector2D(TranslationX, 0.0f));
}

void UMainScreenWidget::HandleHealthChanged(int32 Health, int32 MaxHealth)
{
	if (!HealthBar)
		return;

	HealthBar->SetPercent(static_cast<float>(Health) / static_cast<float>(MaxHealth));
}

void UMainScreenWidget::HandleShieldChanged(int32 Shield, int32 MaxShield)
{
	if (!ShieldBar)
		return;

	ShieldBar->SetPercent(static_cast<float>(Shield) / static_cast<float>(MaxShield));
}
