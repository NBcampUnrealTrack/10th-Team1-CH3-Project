#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "DamageDisplayComponent.generated.h"

class UWidgetComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BEYONDOVERRIDE_API UDamageDisplayComponent : public UActorComponent
{
	GENERATED_BODY()

  protected:
	TObjectPtr<UWidgetComponent> DamageDisplayWidget;

  public:
	UDamageDisplayComponent();

  protected:
	virtual void BeginPlay() override;
};
