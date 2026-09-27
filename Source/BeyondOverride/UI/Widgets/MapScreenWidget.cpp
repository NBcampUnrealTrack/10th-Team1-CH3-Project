#include "UI/Widgets/MapScreenWidget.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Manager/UIManager.h"

void UMapScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsFocusable(true);

	UpdatePlayerMarker();
}

FReply UMapScreenWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Escape || InKeyEvent.GetKey() == EKeys::M)
	{
		if (UUIManager* UIManager = UUIManager::Get(this))
		{
			UIManager->PopScreen();
		}
		return FReply::Handled();
	}

	return FReply::Handled();
}

void UMapScreenWidget::UpdatePlayerMarker()
{
	if (!MapImage || !PlayerMarker)
		return;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!PlayerPawn)
		return;

	const FVector Location = PlayerPawn->GetActorLocation();

	const float AlphaX = (Location.Y - WorldMin.Y) / FMath::Max(WorldMax.Y - WorldMin.Y, 1.f);
	const float AlphaY = (Location.X - WorldMin.X) / FMath::Max(WorldMax.X - WorldMin.X, 1.f);

	FVector2D MapImageSize(800.f, 800.f);
	if (const UCanvasPanelSlot* MapSlot = Cast<UCanvasPanelSlot>(MapImage->Slot))
		MapImageSize = MapSlot->GetSize();

	FVector2D NewPosition((1.f - AlphaX) * MapImageSize.X, AlphaY * MapImageSize.Y);
	NewPosition += MarkerPixelOffset;

	if (UCanvasPanelSlot* MarkerSlot = Cast<UCanvasPanelSlot>(PlayerMarker->Slot))
	{
		MarkerSlot->SetPosition(NewPosition);
	}

	PlayerMarker->SetRenderTransformAngle(PlayerPawn->GetActorRotation().Yaw);
}
