#pragma once

#include "CoreMinimal.h"

#include "Subsystems/GameInstanceSubsystem.h"

#include "ItemDataSubsystem.generated.h"

class UItemDataRegistry;

UCLASS()
class BEYONDOVERRIDE_API UItemDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

  protected:
	TObjectPtr<UItemDataRegistry> ItemDataRegistry;

  public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
};
