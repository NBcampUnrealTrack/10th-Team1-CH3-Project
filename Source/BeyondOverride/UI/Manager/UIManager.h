#pragma once

#include "CoreMinimal.h"

#include "Subsystems/GameInstanceSubsystem.h"

#include "UIManager.generated.h"

UENUM(BlueprintType)
enum class EUIInputMode : uint8
{
	GameOnly,
	UIOnly,
	GameAndUI
};

UENUM(BlueprintType)
enum class EUIScreen : uint8
{
	None,
	Title,     // WBP_TitleScreen
	HUD,       // WBP_MainScreen
	PauseMenu, // WBP_PauseMenuScreen
	Inventory, // WBP_InventoryScreen
	Result     // WBP_ResultScreen
};

UCLASS()
class BEYONDOVERRIDE_API UUIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

  public:
	UUIManager();

	UFUNCTION(BlueprintCallable, Category = "UI", meta = (WorldContext = "WorldContextObject"))
	static UUIManager* Get(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "UI")
	UUserWidget* ShowScreen(EUIScreen Screen, EUIInputMode InputMode = EUIInputMode::GameOnly);

	UFUNCTION(BlueprintCallable, Category = "UI")
	UUserWidget* PushScreen(EUIScreen Screen);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void PopScreen();

	UFUNCTION(BlueprintCallable, Category = "UI")
	UUserWidget* GetCurrentScreen() const;

  private:
	void ApplyInputMode(EUIInputMode InputMode, UUserWidget* Widget);

	UPROPERTY()
	TMap<EUIScreen, TSubclassOf<UUserWidget>> ScreenClasses;

	UPROPERTY()
	UUserWidget* CurrentScreen = nullptr;

	UPROPERTY()
	TArray<UUserWidget*> ScreenStack;
};
