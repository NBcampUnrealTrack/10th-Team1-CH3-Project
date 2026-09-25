// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Widgets/DebugSettingWidget.h"

#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "Engine/TargetPoint.h"
#include "GameFlow/BOGameInstance.h"
#include "GameFlow/Manager/ExitManager.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Character/BOCharacter.h"
#include "Player/PlayerController/BOPlayerController.h"
#include "UI/Manager/UIManager.h"

void UDebugSettingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Locations.Empty();

	if (OKButton)
	{
		OKButton->OnClicked.AddDynamic(this, &UDebugSettingWidget::OnOKButtonClicked);
	}

	SetLocationList();
	SetLocationComboBoxString();
}

void UDebugSettingWidget::SetLocationList()
{
	if (!GetWorld() || !GetWorld()->GetGameInstance())
	{
		return;
	}

	if (UExitManager* ExitManager = GetWorld()->GetGameInstance()->GetSubsystem<UExitManager>())
	{
		ExitManager->GetExitLocations(Locations);
	}

	if (AActor* Actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATargetPoint::StaticClass()))
	{
		ATargetPoint* TargetPoint = Cast<ATargetPoint>(Actor);
		if (TargetPoint && TargetPoint->ActorHasTag(FName(TEXT("AIBuildingEntrance"))))
		{
			FName Name = TargetPoint->Tags[0];
			FVector Location = TargetPoint->GetActorLocation();
			FRotator Rotation = TargetPoint->GetActorRotation();

			Locations.Add(Name, {Location, Rotation});
		}
	}
}

void UDebugSettingWidget::SetLocationComboBoxString()
{
	if (!LocationComboBoxString)
	{
		return;
	}

	for (const TPair<FName, TPair<FVector, FRotator>>& Pair : Locations)
	{
		FString Name = Pair.Key.ToString();

		LocationComboBoxString->AddOption(Name);
	}
}

void UDebugSettingWidget::OnOKButtonClicked()
{
	if (!GetWorld() || !GetWorld()->GetGameInstance())
	{
		return;
	}

	UUIManager* UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();
	if (!UIManager)
	{
		return;
	}

	OnSetLocation();
	OnSetBossDefeated();
	OnSetCharacterSpeed();

	UIManager->PopScreen();
}

void UDebugSettingWidget::OnSetLocation()
{
	if (!LocationComboBoxString || !GetWorld())
	{
		return;
	}

	ABOPlayerController* PlayerController = GetWorld()->GetFirstPlayerController<ABOPlayerController>();
	if (!PlayerController)
	{
		return;
	}

	ABOCharacter* Character = PlayerController->GetPawn<ABOCharacter>();
	if (!Character)
	{
		return;
	}

	FName Option = FName(LocationComboBoxString->GetSelectedOption());

	if (Locations.Contains(Option))
	{
		FVector Location = Locations[Option].Key;
		FRotator Rotation = Locations[Option].Value;

		Character->TeleportTo(Location, Rotation);
		PlayerController->SetControlRotation(Rotation);
	}
}

void UDebugSettingWidget::OnSetBossDefeated()
{
	if (!BossDefeatCheckBox || !GetWorld())
	{
		return;
	}

	UBOGameInstance* GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>();
	if (!GameInstance)
	{
		return;
	}

	ECheckBoxState State = BossDefeatCheckBox->GetCheckedState();

	if (State == ECheckBoxState::Checked)
	{
		GameInstance->SetIsBossDefeated(true);
	}
	else if (State == ECheckBoxState::Unchecked)
	{
		GameInstance->SetIsBossDefeated(false);
	}
}

void UDebugSettingWidget::OnSetCharacterSpeed()
{
	if (!SprintEditableTextBox)
	{
		return;
	}

	ABOCharacter* Character = GetWorld()->GetFirstPlayerController()->GetPawn<ABOCharacter>();
	if (!Character)
	{
		return;
	}

	FString Value = SprintEditableTextBox->GetText().ToString();
	if (Value == "")
	{
		return;
	}

	if (!Value.IsNumeric())
	{
		return;
	}

	float Speed = FCString::Atof(*Value);

	Character->SetSprintSpeed(Speed);
}
