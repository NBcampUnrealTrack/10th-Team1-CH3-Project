#pragma once

#include "CoreMinimal.h"

#include "Subsystems/GameInstanceSubsystem.h"

#include "UIManager.generated.h"

class UInteractPromptWidget;
class UInteractComponent;
class APlayerController;
class UNotificationWidget;

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
	None,           // 기본 (없는 화면도 화면)
	Title,          // WBP_TitleScreen
	HUD,            // WBP_MainScreen
	PauseMenu,      // WBP_PauseMenuScreen
	Inventory,      // WBP_InventoryScreen
	Result,         // WBP_ResultScreen
	FinalResult,    // WBP_FinalResultScreen
	EndingCredits,  // WBP_EndingCredits
	DebugSetting,   // WBP_DebugSetting
	NPCInteraction, // WBP_NPCInteraction
	ShopScreen,     // WBP_ShopScreen
	MapScreen,      // WBP_MapScreen
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMenuOpenStateChanged, bool, bAnyMenuOpen);

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

	UFUNCTION(BlueprintCallable, Category = "UI|Notification")
	void ShowNotification(const FText& Main, const FText& Sub, float Duration = 3.f);

	bool IsAnyMenuOpen() const;

	UPROPERTY(BlueprintAssignable)
	FOnMenuOpenStateChanged OnMenuOpenStateChanged;

  private:
	void ApplyInputMode(EUIInputMode InputMode, UUserWidget* Widget);
	void CenterMouseCursor(APlayerController* PC);
	void NotifyMenuOpenStateChanged();

	bool bLastAnyMenuOpen = false;

	UPROPERTY()
	TMap<EUIScreen, TSubclassOf<UUserWidget>> ScreenClasses;

	UPROPERTY()
	TArray<FUIScreenEntry> ScreenStack;

	UPROPERTY()
	TSubclassOf<UInteractPromptWidget> InteractPromptWidgetClass;

	UPROPERTY()
	UInteractPromptWidget* InteractPromptWidget;

	UPROPERTY()
	TSubclassOf<UNotificationWidget> NotificationWidgetClass;
	UPROPERTY()
	TObjectPtr<UNotificationWidget> NotificationWidget;
};
