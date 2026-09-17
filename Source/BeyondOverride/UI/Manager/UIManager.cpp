#include "UI/Manager/UIManager.h"

#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "Interaction/InteractComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerController/BOPlayerController.h"
#include "UI/Widgets/InteractPromptWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "UI/Widgets/NotificationWidget.h"

UUIManager::UUIManager()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> TitleWBPClass(TEXT("/Game/UI/WBP_TitleScreen"));
	if (TitleWBPClass.Succeeded())
	{
		ScreenClasses.Add(EUIScreen::Title, TitleWBPClass.Class);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> HUDWBPClass(TEXT("/Game/UI/WBP_MainScreen"));
	if (HUDWBPClass.Succeeded())
	{
		ScreenClasses.Add(EUIScreen::HUD, HUDWBPClass.Class);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> PauseWBPClass(TEXT("/Game/UI/WBP_PauseMenuScreen"));
	if (PauseWBPClass.Succeeded())
	{
		ScreenClasses.Add(EUIScreen::PauseMenu, PauseWBPClass.Class);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> InventoryWBPClass(TEXT("/Game/UI/WBP_InventoryScreen"));
	if (InventoryWBPClass.Succeeded())
	{
		ScreenClasses.Add(EUIScreen::Inventory, InventoryWBPClass.Class);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> ResultWBPClass(TEXT("/Game/UI/WBP_ResultScreen"));
	if (ResultWBPClass.Succeeded())
	{
		ScreenClasses.Add(EUIScreen::Result, ResultWBPClass.Class);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> FinalResultWBPClass(TEXT("/Game/UI/WBP_FinalResultScreen"));
	if (FinalResultWBPClass.Succeeded())
	{
		ScreenClasses.Add(EUIScreen::FinalResult, FinalResultWBPClass.Class);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> NoneWBPClass(TEXT("/Game/UI/WBP_None"));
	if (NoneWBPClass.Succeeded())
	{
		ScreenClasses.Add(EUIScreen::None, NoneWBPClass.Class);
	}

	static ConstructorHelpers::FClassFinder<UInteractPromptWidget> InteractPromptWBPClass(TEXT("/Game/UI/WBP_InteractPrompt"));
	if (InteractPromptWBPClass.Succeeded())
	{
		InteractPromptWidgetClass = InteractPromptWBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UNotificationWidget> NotificationWidgetWBPClass(TEXT("/Game/UI/WBP_Notification"));
	if (NotificationWidgetWBPClass.Succeeded())
	{
		NotificationWidgetClass = NotificationWidgetWBPClass.Class;
	}

}

UUIManager* UUIManager::Get(const UObject* WorldContextObject)
{
	if (UGameInstance* GI = UGameplayStatics::GetGameInstance(WorldContextObject))
	{
		return GI->GetSubsystem<UUIManager>();
	}
	return nullptr;
}

UUserWidget* UUIManager::ShowScreen(EUIScreen Screen, EUIInputMode InputMode)
{
	for (const FUIScreenEntry& Entry : ScreenStack)
	{
		if (Entry.Widget)
		{
			Entry.Widget->RemoveFromParent();
		}
	}
	ScreenStack.Empty();

	const TSubclassOf<UUserWidget>* FoundClass = ScreenClasses.Find(Screen);
	if (!FoundClass || !(*FoundClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("없는 화면"));

		return nullptr;
	}

	APlayerController* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
	if (!PC)
		return nullptr;

	UUserWidget* NewWidget = CreateWidget<UUserWidget>(PC, *FoundClass);
	if (NewWidget)
	{
		NewWidget->AddToViewport();
		ScreenStack.Add({NewWidget, Screen, InputMode});
		ApplyInputMode(InputMode, NewWidget);
	}

	NotifyMenuOpenStateChanged();

	return NewWidget;
}

UUserWidget* UUIManager::PushScreen(EUIScreen Screen, EUIInputMode InputMode)
{
	if (ScreenStack.Num() > 0 && ScreenStack.Last().Screen == Screen)
	{
		PopScreen();
		return nullptr;
	}

	const TSubclassOf<UUserWidget>* FoundClass = ScreenClasses.Find(Screen);
	if (!FoundClass || !(*FoundClass))
	{
		return nullptr;
	}

	APlayerController* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
	if (!PC)
		return nullptr;

	UUserWidget* NewWidget = CreateWidget<UUserWidget>(PC, *FoundClass);
	if (NewWidget)
	{
		NewWidget->AddToViewport(ScreenStack.Num() + 1);
		ScreenStack.Add({NewWidget, Screen, InputMode});
		ApplyInputMode(InputMode, NewWidget);

		NotifyMenuOpenStateChanged();
	}

	return NewWidget;
}

void UUIManager::PopScreen()
{
	if (ScreenStack.Num() == 0)
		return;

	UUserWidget* TopWidget = ScreenStack.Last().Widget;
	if (TopWidget)
	{
		TopWidget->RemoveFromParent();
	}

	ScreenStack.RemoveAt(ScreenStack.Num() - 1);

	if (ScreenStack.Num() == 0)
	{
		ApplyInputMode(EUIInputMode::GameOnly, nullptr);
		NotifyMenuOpenStateChanged();
		return;
	}
	const FUIScreenEntry& ChangedCurrent = ScreenStack.Last();
	ApplyInputMode(ChangedCurrent.InputMode, ChangedCurrent.Widget);

	NotifyMenuOpenStateChanged();
}

void UUIManager::ApplyInputMode(EUIInputMode InputMode, UUserWidget* Widget)
{
	APlayerController* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
	if (!PC)
		return;

	switch (InputMode)
	{
	case EUIInputMode::UIOnly:
	{
		FInputModeUIOnly Mode;
		if (Widget)
		{
			Mode.SetWidgetToFocus(Widget->TakeWidget());
		}
		Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(Mode);
		PC->bShowMouseCursor = true;
		CenterMouseCursor(PC);
		break;
	}
	case EUIInputMode::GameAndUI:
	{
		FInputModeGameAndUI Mode;
		if (Widget)
		{
			Mode.SetWidgetToFocus(Widget->TakeWidget());
		}
		PC->SetInputMode(Mode);
		PC->bShowMouseCursor = true;
		CenterMouseCursor(PC);
		break;
	}
	case EUIInputMode::GameOnly:
	default:
		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;
		break;
	}
}

void UUIManager::CenterMouseCursor(APlayerController* PC)
{
	if (!PC || !GEngine || !GEngine->GameViewport)
		return;

	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);

	PC->SetMouseLocation(
		FMath::RoundToInt(ViewportSize.X * 0.5f),
		FMath::RoundToInt(ViewportSize.Y * 0.5f));
}

void UUIManager::NotifyMenuOpenStateChanged()
{
	const bool bAnyMenuOpen = IsAnyMenuOpen();

	if (bLastAnyMenuOpen == bAnyMenuOpen)
	{
		return;
	}

	bLastAnyMenuOpen = bAnyMenuOpen;

	OnMenuOpenStateChanged.Broadcast(IsAnyMenuOpen());
}

void UUIManager::BindInteractPrompt(UInteractComponent* InteractComponent)
{
	if (!InteractComponent)
		return;

	if (!IsValid(InteractPromptWidget) && InteractPromptWidgetClass)
	{
		APlayerController* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
		if (!PC)
			return;

		InteractPromptWidget = CreateWidget<UInteractPromptWidget>(PC, InteractPromptWidgetClass);
	}

	if (InteractPromptWidget)
	{
		if (!InteractPromptWidget->IsInViewport())
		{
			InteractPromptWidget->AddToViewport();
		}

		InteractComponent->OnPromptChanged.AddDynamic(InteractPromptWidget, &UInteractPromptWidget::HandleFocusChanged);
		InteractComponent->OnHoldProgress.AddDynamic(InteractPromptWidget, &UInteractPromptWidget::HandleHoldProgress);
	}
}

bool UUIManager::IsAnyMenuOpen() const
{
	for (const FUIScreenEntry& Entry : ScreenStack)
	{
		if (Entry.Screen != EUIScreen::HUD && Entry.Screen != EUIScreen::None)
		{
			return true;
		}
	}

	return false;
}


void UUIManager::ShowNotification(const FText& Main, const FText& Sub, float Duration)
{
	if (!IsValid(NotificationWidget) && NotificationWidgetClass)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (!PC)
			return;

		NotificationWidget = CreateWidget<UNotificationWidget>(PC, NotificationWidgetClass);
	}

	if (NotificationWidget)
	{
		NotificationWidget->ShowNotification(Main, Sub, Duration);
	}
}
