#include "Player/Character/BOCharacter.h"

#include "EnhancedInputComponent.h"

#include "ActorComponents/EquipmentManagerComponent.h"
#include "Camera/CameraComponent.h"
#include "Enums/EquipmentSlot.h"
#include "Factory/ItemFactory.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interaction/InteractComponent.h"
#include "Items/Actors/ItemPickupBase.h"
#include "Items/Objects/RangeWeaponInstance.h"
#include "Player/ActorComponent/EquipmentComponent.h"
#include "Player/ActorComponent/InventoryComponent.h"
#include "Player/ActorComponent/StatComponent.h"
#include "Player/PlayerController/BOPlayerController.h"

ABOCharacter::ABOCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->SetCrouchedHalfHeight(60.0f);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetArmLength = 250.0f;
	SpringArm->SetRelativeLocation(FVector(0.0f, 20.0f, 90.0f));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = false;
	Camera->SetupAttachment(SpringArm);

	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipementComponent"));
	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractComponent"));
	EquipmentManagerComponent = CreateDefaultSubobject<UEquipmentManagerComponent>(TEXT("EquipmentManagerComponent"));
}

void ABOCharacter::BeginPlay()
{
	Super::BeginPlay();

	ChangeMoveSpeed();

	// EquipmentManagerComponent의 델리게이트 바인딩
	BindingEquipmentManagerComponentDelegates();
}

void ABOCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABOCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ABOPlayerController* PlayerController = Cast<ABOPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &ABOCharacter::Move);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &ABOCharacter::Look);
			}

			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Started, this, &ABOCharacter::StartJump);
				EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Completed, this, &ABOCharacter::StopJump);
			}

			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Started, this, &ABOCharacter::StartSprint);
				EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Completed, this, &ABOCharacter::StopSprint);
				EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Canceled, this, &ABOCharacter::StopSprint);
			}

			if (PlayerController->SeatAction)
			{
				EnhancedInput->BindAction(PlayerController->SeatAction, ETriggerEvent::Started, this, &ABOCharacter::ToggleCrouch);
			}

			if (PlayerController->PrimaryAction)
			{
				EnhancedInput->BindAction(PlayerController->PrimaryAction, ETriggerEvent::Started, this, &ABOCharacter::Primary);
			}

			if (PlayerController->SecondaryAction)
			{
				EnhancedInput->BindAction(PlayerController->SecondaryAction, ETriggerEvent::Started, this, &ABOCharacter::Secondary);
			}

			if (PlayerController->InteractAction)
			{
				EnhancedInput->BindAction(PlayerController->InteractAction, ETriggerEvent::Started, this, &ABOCharacter::InteractPress);
				EnhancedInput->BindAction(PlayerController->InteractAction, ETriggerEvent::Completed, this, &ABOCharacter::InteractRelease);
			}

			if (PlayerController->InventoryAction)
			{
				EnhancedInput->BindAction(PlayerController->InventoryAction, ETriggerEvent::Started, this, &ABOCharacter::Inventory);
			}

			if (PlayerController->EscapeAction)
			{
				EnhancedInput->BindAction(PlayerController->EscapeAction, ETriggerEvent::Started, this, &ABOCharacter::Escape);
			}

			if (PlayerController->EquipSlot1Action)
			{
				EnhancedInput->BindAction(PlayerController->EquipSlot1Action, ETriggerEvent::Started, this, &ABOCharacter::EquipSlot1);
		}
			if (PlayerController->EquipSlot2Action)
			{
				EnhancedInput->BindAction(PlayerController->EquipSlot2Action, ETriggerEvent::Started, this, &ABOCharacter::EquipSlot2);
	}
			if (PlayerController->EquipSlot3Action)
			{
				EnhancedInput->BindAction(PlayerController->EquipSlot3Action, ETriggerEvent::Started, this, &ABOCharacter::EquipSlot3);
}
			if (PlayerController->EquipSlot3Action)
			{
				EnhancedInput->BindAction(PlayerController->EquipSlot4Action, ETriggerEvent::Started, this, &ABOCharacter::EquipSlot4);
			}
			if (PlayerController->EquipSlot4Action)
			{
				EnhancedInput->BindAction(PlayerController->EquipSlot5Action, ETriggerEvent::Started, this, &ABOCharacter::EquipSlot5);
			}

			if (PlayerController->DropEquipmentAction)
			{
				EnhancedInput->BindAction(PlayerController->DropEquipmentAction, ETriggerEvent::Started, this, &ABOCharacter::DropEquipment);
			}

			if (PlayerController->ReloadAction)
			{
				EnhancedInput->BindAction(PlayerController->ReloadAction, ETriggerEvent::Started, this, &ABOCharacter::Reload);
		}
	}
}
}

float ABOCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (IsValid(StatComponent))
	{
		StatComponent->TakeDamage(ActualDamage);
	}

	return ActualDamage;
}

void ABOCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	ChangeMoveSpeed();
}

void ABOCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	ChangeMoveSpeed();
}

void ABOCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	const FVector2D MoveInput = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

void ABOCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void ABOCharacter::StartJump(const FInputActionValue& value)
{
	Jump();
}

void ABOCharacter::StopJump(const FInputActionValue& value)
{
	StopJumping();
}

void ABOCharacter::StartSprint(const FInputActionValue& value)
{
	bIsSprint = true;
	ChangeMoveSpeed();
}

void ABOCharacter::StopSprint(const FInputActionValue& value)
{
	bIsSprint = false;
	ChangeMoveSpeed();
}

void ABOCharacter::ToggleCrouch(const FInputActionValue& value)
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

void ABOCharacter::Primary(const FInputActionValue& value)
{
	if (EquipmentManagerComponent)
	{
		EquipmentManagerComponent->Use();
	}
}

void ABOCharacter::Secondary(const FInputActionValue& value)
{
}

void ABOCharacter::InteractPress(const FInputActionValue& value)
{
	if (IsValid(InteractComponent))
	{
		InteractComponent->PressInteract();
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("111111")));

		// TEMP: 장비 획득 및 장착
		if (AItemPickupBase* ItemPickup = Cast<AItemPickupBase>(InteractComponent->GetFocusedActor()))
		{
			UItemInstanceBase* ItemInstance = ItemPickup->GetItemInstance();
			if (EquipmentManagerComponent)
			{
				EEquipmentSlot ActiveSlot = EquipmentManagerComponent->GetActiveSlot();
				// 현재 빈손인 경우
				if (EquipmentManagerComponent->HasEquipment(ActiveSlot))
				{
					EquipmentManagerComponent->Assign(ActiveSlot, ItemInstance);
				}
				if (!EquipmentManagerComponent->HasEquipment(EEquipmentSlot::Primary))
				{
					EquipmentManagerComponent->Assign(EEquipmentSlot::Primary, ItemInstance);
				}
				else if (!EquipmentManagerComponent->HasEquipment(EEquipmentSlot::Secondary))
				{
					EquipmentManagerComponent->Assign(EEquipmentSlot::Secondary, ItemInstance);
				}
			}
		}
	}
}

void ABOCharacter::InteractRelease(const FInputActionValue& value)
{
	if (IsValid(InteractComponent))
	{
		InteractComponent->ReleaseInteract();
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("222222")));
	}
}

void ABOCharacter::Inventory(const FInputActionValue& value)
{
}

void ABOCharacter::Escape(const FInputActionValue& value)
{
}

void ABOCharacter::EquipSlot1(const FInputActionValue& value)
{
	if (EquipmentManagerComponent)
	{
		EquipmentManagerComponent->Equip(EEquipmentSlot::Primary);
	}
}

void ABOCharacter::EquipSlot2(const FInputActionValue& value)
{
	if (EquipmentManagerComponent)
	{
		EquipmentManagerComponent->Equip(EEquipmentSlot::Secondary);
}
}

void ABOCharacter::EquipSlot3(const FInputActionValue& value)
{
	if (EquipmentManagerComponent)
	{
		EquipmentManagerComponent->Equip(EEquipmentSlot::Melee);
	}
}

void ABOCharacter::EquipSlot4(const FInputActionValue& value)
{
	if (EquipmentManagerComponent)
	{
		EquipmentManagerComponent->Equip(EEquipmentSlot::Throwable);
	}
}

void ABOCharacter::EquipSlot5(const FInputActionValue& value)
{
	if (EquipmentManagerComponent)
	{
		EquipmentManagerComponent->Equip(EEquipmentSlot::Effect);
	}
}

void ABOCharacter::DropEquipment(const FInputActionValue& value)
{
	if (EquipmentManagerComponent)
	{
		// 장비 제거
		UItemInstanceBase* ItemInstance = EquipmentManagerComponent->Unassign(EquipmentManagerComponent->GetActiveSlot());

		// 제거한 장비 액터 소환
		FItemFactory::SpawnItemPickup(
			GetWorld(),
			ItemInstance,
			GetActorLocation() + 30 * GetActorForwardVector());
	}
}

void ABOCharacter::Reload(const FInputActionValue& value)
{
	if (EquipmentManagerComponent)
	{
		EquipmentManagerComponent->Reload();
	}
}

void ABOCharacter::ChangeMoveSpeed()
{
	float NewMoveSpeed = bIsSprint ? SprintSpeed : WalkSpeed;

	GetCharacterMovement()->MaxWalkSpeed = NewMoveSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = NewMoveSpeed * CrouchSpeedMultiplier;
}

void ABOCharacter::OnEquipmentSlotChanged(EEquipmentSlot Slot, UItemInstanceBase* ItemInstanceBase)
{
	if (ItemInstanceBase)
	{
		EquipmentManagerComponent->Assign(Slot, ItemInstanceBase);
	}
	else
	{
		EquipmentManagerComponent->Unassign(Slot);
	}
}

void ABOCharacter::BindingEquipmentManagerComponentDelegates()
{
	if (!EquipmentManagerComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ABOCharacter] EquipmentManagerComponent의 델리게이트 바인딩 실패 : 유효하지 않은 EquipmentManagerComponent"));
		return;
	}

	// Primary & Secondary (Range Weapon)
	EquipmentManagerComponent->CanReloadDelegate.BindUObject(this, &ABOCharacter::OnCanReload);
	EquipmentManagerComponent->RequestReloadAmmoDelegate.BindUObject(this, &ABOCharacter::OnRequestReloadAmmo);
}

bool ABOCharacter::OnCanReload(URangeWeaponInstance* RangeWeaponInstance) const
{
	// TEMP: 재장전 항상 가능
	return true;
}

int32 ABOCharacter::OnRequestReloadAmmo(URangeWeaponInstance* RangeWeaponInstance)
{
	// TEMP: 재장전 탄약 충분
	return 100;
}
