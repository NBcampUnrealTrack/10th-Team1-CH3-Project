#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "EquipmentHandlerComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BEYONDOVERRIDE_API UEquipmentHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

  public:
	UEquipmentHandlerComponent();
};
