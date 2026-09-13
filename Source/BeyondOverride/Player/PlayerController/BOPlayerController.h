#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"

#include "BOPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class BEYONDOVERRIDE_API ABOPlayerController : public APlayerController
{
	GENERATED_BODY()

  public:
	void ShowMainHUDWidget();
	void ShowESCWidget();

  public:
	ABOPlayerController();

  protected:
	virtual void BeginPlay() override;

  public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* InputMappingContext = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SprintAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SeatAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* PrimaryAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SecondaryAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InteractAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InventoryAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* EscapeAction = nullptr;

	// 장비 Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Equipment")
	UInputAction* EquipSlot1Action = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Equipment")
	UInputAction* EquipSlot2Action = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Equipment")
	UInputAction* EquipSlot3Action = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Equipment")
	UInputAction* EquipSlot4Action = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Equipment")
	UInputAction* EquipSlot5Action = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Equipment")
	UInputAction* DropEquipmentAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Equipment")
	UInputAction* ReloadAction = nullptr;

  protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UUserWidget* MainHUDWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UUserWidget* EscapeWidget;
};
