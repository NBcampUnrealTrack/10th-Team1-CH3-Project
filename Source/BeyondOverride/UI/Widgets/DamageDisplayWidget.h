#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "DamageDisplayWidget.generated.h"

class UTextBlock;

UCLASS()
class BEYONDOVERRIDE_API UDamageDisplayWidget : public UUserWidget
{
	GENERATED_BODY()

  protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DamageText;

  public:
	void SetDamageNumber(int32 TotalDamage);
};
