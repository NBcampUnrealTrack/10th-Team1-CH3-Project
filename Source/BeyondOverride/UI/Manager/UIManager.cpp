#include "UI/Manager/UIManager.h"

#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

UUIManager::UUIManager(){
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
	const TSubclassOf<UUserWidget>* FoundClass = ScreenClasses.Find(Screen);
	if (!FoundClass || !(*FoundClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("없는 화면"));
		return nullptr;
	}

	for (UUserWidget* StackedWidget : ScreenStack)
	{
		if (StackedWidget)
		{
			StackedWidget->RemoveFromParent();
		}
	}
	ScreenStack.Empty();

	if (CurrentScreen)
	{
		CurrentScreen->RemoveFromParent();
		CurrentScreen = nullptr;
	}

	UGameInstance* GI = GetGameInstance();
	if (!GI)
		return nullptr;

	UUserWidget* NewWidget = CreateWidget<UUserWidget>(GI, *FoundClass);
	if (NewWidget)
	{
		NewWidget->AddToViewport();
		CurrentScreen = NewWidget;
		ApplyInputMode(InputMode, NewWidget);
	}

	return NewWidget;
}

UUserWidget* UUIManager::PushScreen(EUIScreen Screen)
{
	const TSubclassOf<UUserWidget>* FoundClass = ScreenClasses.Find(Screen);
	if (!FoundClass || !(*FoundClass))
	{
		return nullptr;
	}

	UGameInstance* GI = GetGameInstance();
	if (!GI)
		return nullptr;

	UUserWidget* NewWidget = CreateWidget<UUserWidget>(GI, *FoundClass);
	if (NewWidget)
	{
		NewWidget->AddToViewport(ScreenStack.Num() + 1);
		ScreenStack.Add(NewWidget);
	}

	return NewWidget;
}

void UUIManager::PopScreen()
{
	if (ScreenStack.Num() == 0)
		return;

	UUserWidget* TopWidget = ScreenStack.Last();
	if (TopWidget)
	{
		TopWidget->RemoveFromParent();
	}

	ScreenStack.RemoveAt(ScreenStack.Num() - 1);
}

UUserWidget* UUIManager::GetCurrentScreen() const
{
	return CurrentScreen;
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
		Mode.SetWidgetToFocus(Widget->TakeWidget());
		Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(Mode);
		PC->bShowMouseCursor = true;
		break;
	}
	case EUIInputMode::GameAndUI:
	{
		FInputModeGameAndUI Mode;
		Mode.SetWidgetToFocus(Widget->TakeWidget());
		PC->SetInputMode(Mode);
		PC->bShowMouseCursor = true;
		break;
	}
	case EUIInputMode::GameOnly:
	default:
		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;
		break;
	}
}
