#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "KillCountEntryWidget.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class BEYONDOVERRIDE_API UKillCountEntryWidget : public UUserWidget
{
	GENERATED_BODY()

  public:
	UFUNCTION()
	void SetKillCountEntry(const FText& InMonsterName, int32 InMonsterKillCount);

  private:
	UPROPERTY(EditDefaultsOnly, Category = "Monster")
	TObjectPtr<UDataTable> MonsterDataTable;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> MonsterImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MonsterNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MonsterKillCountText;
};
