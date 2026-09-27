#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "MapScreenWidget.generated.h"

class UImage;
class UCanvasPanel;

UCLASS()
class BEYONDOVERRIDE_API UMapScreenWidget : public UUserWidget
{
	GENERATED_BODY()

  protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	FVector2D WorldMin = FVector2D(-151185.764937f, -151196.751633);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	FVector2D WorldMax = FVector2D(151202.377948, 151201.986827);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	FVector2D MarkerPixelOffset = FVector2D(0.f, 0.f);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> MapImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> PlayerMarker;

  private:
	void UpdatePlayerMarker();
};
