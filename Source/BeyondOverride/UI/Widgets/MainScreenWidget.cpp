#include "MainScreenWidget.h"

#include "ActorComponents/EquipmentManagerComponent.h"
#include "Blueprint/WidgetTree.h"
#include "Components/PanelWidget.h"
#include "Components/ProgressBar.h"
#include "Enums/EquipmentSlot.h"
#include "Items/Objects/ItemInstanceBase.h"
#include "Player/ActorComponent/PlayerInventoryComponent.h"
#include "Player/ActorComponent/StatComponent.h"
#include "Player/Character/BOCharacter.h"
#include "UI/Widgets/EquipmentSlotWidget.h"

void UMainScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OwningCharacter = Cast<ABOCharacter>(GetOwningPlayerPawn());
	if (!OwningCharacter)
		return;

	CurrentCrosshairOffset = CrosshairBaseOffset;
	EquipmentManager = OwningCharacter->GetEquipmentComponent();
	if (EquipmentManager)
	{
		EquipmentManager->OnSpreadDegreeUpdatedDelegate.AddUObject(this, &UMainScreenWidget::HandleSpreadDegreeUpdated);
	}

	StatComponent = OwningCharacter->GetStatComponent();
	if (!StatComponent)
		return;

	StatComponent->OnHealthChanged.AddDynamic(this, &UMainScreenWidget::HandleHealthChanged);
	StatComponent->OnShieldChanged.AddDynamic(this, &UMainScreenWidget::HandleShieldChanged);

	InventoryComponent = OwningCharacter->GetPlayerInventoryComponent();
	if (InventoryComponent)
	{
		InventoryComponent->OnEquipmentSlotChanged.AddDynamic(this, &UMainScreenWidget::HandleEquipmentSlotChanged);

		HandleEquipmentSlotChanged(EEquipmentSlot::Shield, InventoryComponent->GetEquipmentItem(EEquipmentSlot::Shield));
	}

	HandleHealthChanged(StatComponent->GetCurHealth(), StatComponent->GetMaxHealth());
	HandleShieldChanged(StatComponent->GetCurShield(), StatComponent->GetMaxShield());

	if (WidgetTree)
	{
		WidgetTree->ForEachWidget([this](UWidget* Widget)
								  {
			if (UEquipmentSlotWidget* EquipmentSlot = Cast<UEquipmentSlotWidget>(Widget))
			{
				EquipmentSlot->SetupEquipmentSlot(
					OwningCharacter->GetPlayerInventoryComponent(),
					OwningCharacter->GetInventoryInteractionComponent(),
					OwningCharacter->GetEquipmentComponent());
			} });
	}
}

void UMainScreenWidget::NativeDestruct()
{
	if (EquipmentManager)
	{
		EquipmentManager->OnSpreadDegreeUpdatedDelegate.RemoveAll(this);
		EquipmentManager = nullptr;
	}

	if (StatComponent)
	{
		StatComponent->OnHealthChanged.RemoveDynamic(this, &UMainScreenWidget::HandleHealthChanged);
		StatComponent->OnShieldChanged.RemoveDynamic(this, &UMainScreenWidget::HandleShieldChanged);
		StatComponent = nullptr;
	}

	if (InventoryComponent)
	{
		InventoryComponent->OnEquipmentSlotChanged.RemoveDynamic(this, &UMainScreenWidget::HandleEquipmentSlotChanged);
		InventoryComponent = nullptr;
	}

	Super::NativeDestruct();
}

void UMainScreenWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateCompass();
	UpdateCrosshair(InDeltaTime);
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

void UMainScreenWidget::HandleSpreadDegreeUpdated(float SpreadDegree)
{
	TargetSpreadDegree = SpreadDegree;
}

void UMainScreenWidget::UpdateCrosshair(float DeltaTime)
{
	// 목표 거리 = 기본 거리 + (탄 퍼짐 각도 * 도당 픽셀), 최대값으로 제한
	const float TargetOffset = FMath::Min(
		CrosshairBaseOffset + TargetSpreadDegree * CrosshairPixelsPerDegree,
		CrosshairMaxOffset);

	// 부드럽게 따라가도록 보간
	CurrentCrosshairOffset = FMath::FInterpTo(CurrentCrosshairOffset, TargetOffset, DeltaTime, CrosshairInterpSpeed);

	// 위젯은 WBP에서 정중앙에 배치해 두고, 이동량만 Render Translation으로 준다
	if (LineTop)
		LineTop->SetRenderTranslation(FVector2D(0.0f, -CurrentCrosshairOffset));
	if (LineBottom)
		LineBottom->SetRenderTranslation(FVector2D(0.0f, CurrentCrosshairOffset));
	if (LineLeft)
		LineLeft->SetRenderTranslation(FVector2D(-CurrentCrosshairOffset, 0.0f));
	if (LineRight)
		LineRight->SetRenderTranslation(FVector2D(CurrentCrosshairOffset, 0.0f));
}

void UMainScreenWidget::HandleHealthChanged(int32 Health, int32 MaxHealth)
{
	if (!HealthBar)
		return;

	float Percent = MaxHealth > 0 ? static_cast<float>(Health) / static_cast<float>(MaxHealth) : 0.0f;
	HealthBar->SetPercent(Percent);
}

void UMainScreenWidget::HandleShieldChanged(int32 Shield, int32 MaxShield)
{
	if (!ShieldBar)
		return;

	float Percent = MaxShield > 0 ? static_cast<float>(Shield) / static_cast<float>(MaxShield) : 0.0f;
	ShieldBar->SetPercent(Percent);
}

void UMainScreenWidget::HandleEquipmentSlotChanged(EEquipmentSlot ChangedSlot, UItemInstanceBase* ItemInstanceBase)
{
	if (ChangedSlot != EEquipmentSlot::Shield || !ShieldBar)
		return;

	ShieldBar->SetVisibility(IsValid(ItemInstanceBase) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}
