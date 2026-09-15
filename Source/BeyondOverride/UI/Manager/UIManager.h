#pragma once

#include "CoreMinimal.h"

#include "Subsystems/GameInstanceSubsystem.h"

#include "UIManager.generated.h"

class UInteractPromptWidget;
class UInteractComponent;
class APlayerController;

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
	None,      // 기본 (없는 화면도 화면)
	Title,     // WBP_TitleScreen
	HUD,       // WBP_MainScreen
	PauseMenu, // WBP_PauseMenuScreen
	Inventory, // WBP_InventoryScreen
	Result     // WBP_ResultScreen
};

USTRUCT()
struct FUIScreenEntry
{
	GENERATED_BODY()

	FUIScreenEntry() = default;

	FUIScreenEntry(UUserWidget* InWidget, EUIScreen InScreen, EUIInputMode InInputMode)
		: Widget(InWidget), Screen(InScreen), InputMode(InInputMode)
	{
	}

	UPROPERTY()
	TObjectPtr<UUserWidget> Widget = nullptr;

	UPROPERTY()
	EUIScreen Screen = EUIScreen::None;

	UPROPERTY()
	EUIInputMode InputMode = EUIInputMode::GameOnly;
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
	UUserWidget* PushScreen(EUIScreen Screen, EUIInputMode InputMode = EUIInputMode::UIOnly);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void PopScreen();

	void BindInteractPrompt(UInteractComponent* InteractComponent);

  private:
	void ApplyInputMode(EUIInputMode InputMode, UUserWidget* Widget);
	void CenterMouseCursor(APlayerController* PC);

	UPROPERTY()
	TMap<EUIScreen, TSubclassOf<UUserWidget>> ScreenClasses;

	UPROPERTY()
	TArray<FUIScreenEntry> ScreenStack;

	UPROPERTY()
	TSubclassOf<UInteractPromptWidget> InteractPromptWidgetClass;

	UPROPERTY()
	UInteractPromptWidget* InteractPromptWidget;
};
