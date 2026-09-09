#include "Player/Character/BOCharacter.h"

#include "Player/PlayerController/BOPlayerController.h"
#include "EnhancedInputComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Player/ActorComponent/EquipmentComponent.h"
#include "Player/ActorComponent/StatComponent.h"
#include "Player/ActorComponent/InventoryComponent.h"

ABOCharacter::ABOCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = false;
	Camera->SetupAttachment(SpringArm);

	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipementComponent"));
	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

void ABOCharacter::BeginPlay()
{
	Super::BeginPlay();

	ChangeMoveSpeed();
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
				EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Triggered, this, &ABOCharacter::StartJump);
				EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Completed, this, &ABOCharacter::StopJump);
			}

			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Triggered, this, &ABOCharacter::StartSprint);
				EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Completed, this, &ABOCharacter::StopSprint);
			}

			if (PlayerController->SeatAction)
			{
				EnhancedInput->BindAction(PlayerController->SeatAction, ETriggerEvent::Triggered, this, &ABOCharacter::ToggleSeat);
			}

			if (PlayerController->PrimaryAction)
			{
				EnhancedInput->BindAction(PlayerController->PrimaryAction, ETriggerEvent::Triggered, this, &ABOCharacter::Primary);
			}

			if (PlayerController->SecondaryAction)
			{
				EnhancedInput->BindAction(PlayerController->SecondaryAction, ETriggerEvent::Triggered, this, &ABOCharacter::Secondary);
			}

			if (PlayerController->InteractAction)
			{
				EnhancedInput->BindAction(PlayerController->InteractAction, ETriggerEvent::Triggered, this, &ABOCharacter::Interact);
			}

			if (PlayerController->InventoryAction)
			{
				EnhancedInput->BindAction(PlayerController->InventoryAction, ETriggerEvent::Triggered, this, &ABOCharacter::Inventory);
			}

			if (PlayerController->EscapeAction)
			{
				EnhancedInput->BindAction(PlayerController->EscapeAction, ETriggerEvent::Triggered, this, &ABOCharacter::Escape);
			}
		}
	}
}

float ABOCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return 0.0f;
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
	if (value.Get<bool>())
	{
		Jump();
	}
}

void ABOCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
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

void ABOCharacter::ToggleSeat(const FInputActionValue& value)
{
	bIsSeat = !bIsSeat;
	ChangeMoveSpeed();
}

void ABOCharacter::Primary(const FInputActionValue& value)
{
}

void ABOCharacter::Secondary(const FInputActionValue& value)
{
}

void ABOCharacter::Interact(const FInputActionValue& value)
{
}

void ABOCharacter::Inventory(const FInputActionValue& value)
{
}

void ABOCharacter::Escape(const FInputActionValue& value)
{
}

void ABOCharacter::ChangeMoveSpeed()
{
	float NewMoveSpeed = bIsSprint ? SprintSpeed : WalkSpeed;

	if (bIsSeat)
	{
		NewMoveSpeed *= SeatSpeedMultiplier;
	}

	GetCharacterMovement()->MaxWalkSpeed = NewMoveSpeed;
}
