#include "Player/Character/BOCharacter.h"

#include "EnhancedInputComponent.h"

#include "ActorComponents/EquipmentManagerComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Camera/CameraComponent.h"
#include "DataTables/Items/EquippableItemDataRow.h"
#include "Enums/EquipmentSlot.h"
#include "Factory/ItemFactory.h"
#include "GameFlow/BOGameInstance.h"
#include "GameFlow/BOGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interaction/InteractComponent.h"
#include "Items/Actors/ItemPickupBase.h"
#include "Items/Objects/EquippableItemInstance.h"
#include "Items/Objects/MeleeWeaponInstance.h"
#include "Items/Objects/RangeWeaponInstance.h"
#include "Items/Objects/ThrowableItemInstance.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"
#include "Player/ActorComponent/EquipmentComponent.h"
#include "Player/ActorComponent/InventoryInteractionComponent.h"
#include "Player/ActorComponent/CharacterPreviewComponent.h"
#include "Player/ActorComponent/NearbyItemComponent.h"
#include "Player/ActorComponent/PlayerInventoryComponent.h"
#include "Player/ActorComponent/StatComponent.h"
#include "Player/AnimInstance/BOAnimInstance.h"
#include "Player/PlayerController/BOPlayerController.h"
#include "UI/Manager/UIManager.h"

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

	// EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipementComponent"));
	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
	PlayerInventoryComponent = CreateDefaultSubobject<UPlayerInventoryComponent>(TEXT("InventoryComponent"));
	InventoryInteractionComponent = CreateDefaultSubobject<UInventoryInteractionComponent>(TEXT("InventoryInteractionComponent"));
	InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractComponent"));
	NearbyItemComponent = CreateDefaultSubobject<UNearbyItemComponent>(TEXT("NearbyItemComponent"));
	EquipmentManagerComponent = CreateDefaultSubobject<UEquipmentManagerComponent>(TEXT("EquipmentManagerComponent"));

	// 프리뷰 추가
	CharacterPreviewComponent = CreateDefaultSubobject<UCharacterPreviewComponent>(TEXT("CharacterPreviewComponent"));
}

void ABOCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(StatComponent))
	{
		StatComponent->OnDeath.AddUObject(this, &ABOCharacter::HandleDeath);
	}

	if (UUIManager* UIManager = UUIManager::Get(this))
	{
		UIManager->OnMenuOpenStateChanged.AddDynamic(this, &ABOCharacter::OnMenuOpenStateChanged);
		OnMenuOpenStateChanged(UIManager->IsAnyMenuOpen());
	}

	if (IsValid(Camera))
	{
		DefaultFOV = Camera->FieldOfView;
	}

	ChangeMoveSpeed();

	// EquipmentManagerComponent 설정
	BindingEquipmentManagerComponentDelegates(); // 델리게이트 바인딩
	EquipmentManagerComponent->Initialize();     // 초기 설정

	if (IsValid(PlayerInventoryComponent))
	{
		PlayerInventoryComponent->OnEquipmentItemChanged.AddDynamic(this, &ABOCharacter::OnEquipmentItemChanged);
	}

	if (UUIManager* UIManager = UUIManager::Get(this))
	{
		UIManager->BindInteractPrompt(InteractComponent);
	}

	if (GetWorld())
	{
		if (UBOGameInstance* GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>())
		{
			if (GameInstance->GetGameState() == EGameState::Playing)
			{
				if (StatComponent)
				{
					UE_LOG(LogTemp, Warning, TEXT("Load Stat Component"));
					StatComponent->SetCurHealth(GameInstance->GetCurHealth());
					StatComponent->SetMaxHealth(GameInstance->GetMaxHealth());
					StatComponent->SetCurShield(GameInstance->GetCurShield());
					StatComponent->SetMaxShield(GameInstance->GetMaxShield());
				}

				if (PlayerInventoryComponent)
				{
					PlayerInventoryComponent->SetSlots(GameInstance->GetPlayerItemInventory());
					PlayerInventoryComponent->SetEquipmentSlots(GameInstance->GetPlayerEquipmentInventory());
				}
			}
		}
	}
}

void ABOCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(StatComponent))
	{
		StatComponent->OnDeath.RemoveAll(this);
	}

	if (UUIManager* UIManager = UUIManager::Get(this))
	{
		UIManager->OnMenuOpenStateChanged.RemoveDynamic(this, &ABOCharacter::OnMenuOpenStateChanged);
	}

	Super::EndPlay(EndPlayReason);
}

void ABOCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsRolling)
	{
		UCharacterMovementComponent* MovementComponent = GetCharacterMovement();

		if (!IsValid(MovementComponent))
		{
			return;
		}

		const FVector RollVelocity = RollDirection * RollSpeed;

		MovementComponent->Velocity.X = RollVelocity.X;
		MovementComponent->Velocity.Y = RollVelocity.Y;
	}

	if (IsValid(Camera))
	{
		const float TargetFOV = bIsAiming ? AimFOV : DefaultFOV;
		const float NewFOV = FMath::FInterpTo(Camera->FieldOfView, TargetFOV, DeltaTime, ZoomSpeed);

		Camera->SetFieldOfView(NewFOV);
	}
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
				EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Completed, this, &ABOCharacter::Move);
				EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Canceled, this, &ABOCharacter::Move);
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
				EnhancedInput->BindAction(PlayerController->PrimaryAction, ETriggerEvent::Triggered, this, &ABOCharacter::Fire);
				EnhancedInput->BindAction(PlayerController->PrimaryAction, ETriggerEvent::Started, this, &ABOCharacter::StartFire);
				EnhancedInput->BindAction(PlayerController->PrimaryAction, ETriggerEvent::Completed, this, &ABOCharacter::CompleteFire);
			}

			if (PlayerController->SecondaryAction)
			{
				EnhancedInput->BindAction(PlayerController->SecondaryAction, ETriggerEvent::Started, this, &ABOCharacter::Aim);
				EnhancedInput->BindAction(PlayerController->SecondaryAction, ETriggerEvent::Completed, this, &ABOCharacter::Hip);
			}

			if (PlayerController->ReloadAction)
			{
				EnhancedInput->BindAction(PlayerController->ReloadAction, ETriggerEvent::Started, this, &ABOCharacter::Reload);
			}

			if (PlayerController->RollAction)
			{
				EnhancedInput->BindAction(PlayerController->RollAction, ETriggerEvent::Started, this, &ABOCharacter::Roll);
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
			if (PlayerController->EquipSlot4Action)
			{
				EnhancedInput->BindAction(PlayerController->EquipSlot4Action, ETriggerEvent::Started, this, &ABOCharacter::EquipSlot4);
			}
			if (PlayerController->EquipSlot5Action)
			{
				EnhancedInput->BindAction(PlayerController->EquipSlot5Action, ETriggerEvent::Started, this, &ABOCharacter::EquipSlot5);
			}

			if (PlayerController->UnarmAction)
			{
				EnhancedInput->BindAction(PlayerController->UnarmAction, ETriggerEvent::Started, this, &ABOCharacter::Unarm);
			}

			if (PlayerController->DropEquipmentAction)
			{
				EnhancedInput->BindAction(PlayerController->DropEquipmentAction, ETriggerEvent::Started, this, &ABOCharacter::DropEquipment);
			}
		}
	}
}

float ABOCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (IsValid(StatComponent))
	{
		StatComponent->TakeDamage(ActualDamage, DamageCauser);
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

void ABOCharacter::SetMovementEnabled(bool bEnabled)
{
	if (bMovementEnabled == bEnabled)
	{
		return;
	}

	bMovementEnabled = bEnabled;

	AController* CharacterController = GetController();

	if (IsValid(CharacterController))
	{
		CharacterController->SetIgnoreMoveInput(!bEnabled);
	}

	if (!bEnabled)
	{
		bIsSprint = false;

		if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
		{
			MovementComponent->StopMovementImmediately();
		}

		ChangeMoveSpeed();
	}
}

void ABOCharacter::Move(const FInputActionValue& value)
{
	if (!Controller)
	{
		return;
	}

	MoveInput = value.Get<FVector2D>();

	if (!bMovementEnabled || bIsRolling)
	{
		return;
	}

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
	if (bIsRolling || !bMovementEnabled)
	{
		return;
	}

	Jump();
}

void ABOCharacter::StopJump(const FInputActionValue& value)
{
	StopJumping();
}

void ABOCharacter::StartSprint(const FInputActionValue& value)
{
	if (!bMovementEnabled)
	{
		return;
	}

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
	if (!bMovementEnabled)
	{
		return;
	}

	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

void ABOCharacter::Fire(const FInputActionValue& value)
{
	if (bIsRolling)
	{
		return;
	}

	// 현재 장비 사용 시도
	if (!EquipmentManagerComponent || !EquipmentManagerComponent->Use())
	{
		return;
	}

	if (!GetMesh() || !GetMesh()->GetAnimInstance())
	{
		return;
	}

	UBOAnimInstance* AnimInstance = Cast<UBOAnimInstance>(GetMesh()->GetAnimInstance());
	if (!IsValid(AnimInstance))
	{
		return;
	}

	if (bIsAiming)
	{
		AnimInstance->PlayFireAimMontage();
	}
	else
	{
		AnimInstance->PlayFireHipMontage();
	}
}

void ABOCharacter::StartFire(const FInputActionValue& value)
{
	if (bIsRolling)
	{
		return;
	}

	// 장비 사용 시작
	if (EquipmentManagerComponent)
	{
		EquipmentManagerComponent->StartAction();
	}
}

void ABOCharacter::CompleteFire(const FInputActionValue& value)
{
	// 장비 사용 종료
	if (EquipmentManagerComponent)
	{
		EquipmentManagerComponent->EndAction();
	}
}

void ABOCharacter::Hip(const FInputActionValue& value)
{
	bIsAiming = false;
}

void ABOCharacter::Reload(const FInputActionValue& value)
{
	if (bIsRolling)
	{
		return;
	}

	// 장비 재장전
	if (!IsValid(EquipmentManagerComponent) || !EquipmentManagerComponent->Reload())
	{
		return;
	}

	if (!GetMesh() || !GetMesh()->GetAnimInstance())
	{
		return;
	}

	UBOAnimInstance* AnimInstance = Cast<UBOAnimInstance>(GetMesh()->GetAnimInstance());
	if (!IsValid(AnimInstance))
	{
		return;
	}

	if (bIsAiming)
	{
		AnimInstance->PlayReloadAimMontage();
	}
	else
	{
		AnimInstance->PlayReloadHipMontage();
	}
}

void ABOCharacter::Roll(const FInputActionValue& Value)
{
	/*if (!IsValid(RollMontage) || !IsValid(GetMesh()))
	{
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (!IsValid(AnimInstance))
	{
		return;
	}

	if (AnimInstance->Montage_IsPlaying(RollMontage))
	{
		return;
	}

	const FName SectionName = GetRollSectionName();

	const float Duration = AnimInstance->Montage_Play(RollMontage);

	if (Duration <= 0.0f)
	{
		return;
	}

	AnimInstance->Montage_JumpToSection(SectionName, RollMontage);*/

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();

	if (bIsRolling || !IsValid(MovementComponent) || MovementComponent->IsFalling() || !IsValid(RollMontage) || !IsValid(GetMesh()))
	{
		return;
	}

	UAnimInstance* Anim = GetMesh()->GetAnimInstance();

	if (!IsValid(Anim))
	{
		return;
	}

	UBOAnimInstance* AnimInstance = Cast<UBOAnimInstance>(Anim);

	if (!IsValid(AnimInstance))
	{
		return;
	}

	if (AnimInstance->IsReloadMontagePlaying())
	{
		return;
	}

	const FName SectionName = GetRollSectionName();

	RollDirection = GetRollDirection();
	ConsumeMovementInputVector();
	StartRoll();

	const float Duration = AnimInstance->Montage_Play(RollMontage);

	if (Duration <= 0.0f)
	{
		StopRoll();
		return;
	}

	AnimInstance->Montage_JumpToSection(SectionName, RollMontage);

	FOnMontageEnded MontageEndedDelegate;

	MontageEndedDelegate.BindUObject(this, &ABOCharacter::OnRollMontageEnded);

	AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, RollMontage);
}

void ABOCharacter::StartRoll()
{
	bIsRolling = true;

	// 장비 사용 종료
	if (EquipmentManagerComponent)
	{
		EquipmentManagerComponent->EndAction();
	}
}

void ABOCharacter::StopRoll()
{
	bIsRolling = false;
	RollDirection = FVector::ZeroVector;
}

void ABOCharacter::Aim(const FInputActionValue& value)
{
	bIsAiming = true;
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
			if (UItemInstanceBase* ItemInstance = ItemPickup->GetItemInstance())
			{
				if (IsValid(PlayerInventoryComponent))
				{
					if (PlayerInventoryComponent->AddItem(ItemInstance))
					{
						ItemPickup->Destroy();
					}
				}

				if (EquipmentManagerComponent)
				{

					//// Range Weapon
					// if (ItemInstance->IsA(URangeWeaponInstance::StaticClass()))
					//{
					//	if (EquipmentManagerComponent->Assign(EEquipmentSlot::Primary, ItemInstance))
					//	{
					//		if (PlayerInventoryComponent)
					//		{
					//			PlayerInventoryComponent->SetEquipmentItem(EEquipmentSlot::Primary, ItemInstance);
					//		}
					//		ItemPickup->Destroy();
					//	}
					//	else if (EquipmentManagerComponent->Assign(EEquipmentSlot::Secondary, ItemInstance))
					//	{
					//		if (PlayerInventoryComponent)
					//		{
					//			PlayerInventoryComponent->SetEquipmentItem(EEquipmentSlot::Secondary, ItemInstance);
					//		}
					//		ItemPickup->Destroy();
					//	}
					// }
					//// Melee Weapon
					// else if (ItemInstance->IsA(UMeleeWeaponInstance::StaticClass()))
					//{
					//	if (EquipmentManagerComponent->Assign(EEquipmentSlot::Melee, ItemInstance))
					//	{
					//		if (PlayerInventoryComponent)
					//		{
					//			PlayerInventoryComponent->SetEquipmentItem(EEquipmentSlot::Melee, ItemInstance);
					//		}
					//		ItemPickup->Destroy();
					//	}
					// }
					////// throwable Weapon
					////else if (ItemInstance->IsA(UMeleeWeaponInstance::StaticClass()))
					////{
					////	if (EquipmentManagerComponent->Assign(EEquipmentSlot::Melee, ItemInstance))
					////	{
					////		ItemPickup->Destroy();
					////	}
					////}
					// else
					//{
					//	if (PlayerInventoryComponent->AddItem(ItemInstance))
					//	{
					//		ItemPickup->Destroy();
					//	}
					// }
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
	if (UUIManager* UIManager = UUIManager::Get(this))
	{
		UIManager->PushScreen(EUIScreen::Inventory, EUIInputMode::GameAndUI);
	}
}

void ABOCharacter::Escape(const FInputActionValue& value)
{
	if (UUIManager* UIManager = UUIManager::Get(this))
	{
		UIManager->PushScreen(EUIScreen::PauseMenu, EUIInputMode::UIOnly);
	}
}

void ABOCharacter::EquipSlot1(const FInputActionValue& value)
{
	TryEquipSlot(EEquipmentSlot::Primary);
}

void ABOCharacter::EquipSlot2(const FInputActionValue& value)
{
	TryEquipSlot(EEquipmentSlot::Secondary);
}

void ABOCharacter::EquipSlot3(const FInputActionValue& value)
{
	TryEquipSlot(EEquipmentSlot::Melee);
}

void ABOCharacter::EquipSlot4(const FInputActionValue& value)
{
	TryEquipSlot(EEquipmentSlot::Throwable);
}

void ABOCharacter::EquipSlot5(const FInputActionValue& value)
{
	TryEquipSlot(EEquipmentSlot::Effect);
}

void ABOCharacter::Unarm(const FInputActionValue& value)
{
	if (EquipmentManagerComponent)
	{
		EquipmentManagerComponent->Unequip();
	}
}

void ABOCharacter::DropEquipment(const FInputActionValue& value)
{
	// if (EquipmentManagerComponent)
	//{
	//	// 장비 제거
	//	UItemInstanceBase* ItemInstance = EquipmentManagerComponent->Unassign(EquipmentManagerComponent->GetActiveSlot());

	//	// 제거한 장비 액터 소환
	//	FItemFactory::SpawnItemPickup(
	//		GetWorld(),
	//		ItemInstance,
	//		GetActorLocation() + 30 * GetActorForwardVector());
	//}

	if (!IsValid(PlayerInventoryComponent) || !IsValid(EquipmentManagerComponent))
	{
		return;
	}

	const EEquipmentSlot ActiveSlot = EquipmentManagerComponent->GetActiveSlot();

	// 맨손은 버릴 수 없음
	if (ActiveSlot == EEquipmentSlot::Unarmed)
	{
		return;
	}

	UItemInstanceBase* ItemInstance = PlayerInventoryComponent->GetEquipmentItem(ActiveSlot);

	if (!IsValid(ItemInstance))
	{
		return;
	}

	// 인벤토리 슬롯을 비우면 OnEquipmentSlotChanged가 발생하고 EquipmentManager가 이를 받아 실제 장비를 제거
	if (!PlayerInventoryComponent->SetEquipmentItem(ActiveSlot, nullptr))
	{
		return;
	}

	FItemFactory::SpawnItemPickup(GetWorld(), ItemInstance, GetActorLocation() + 30.0f * GetActorForwardVector());
}

void ABOCharacter::ChangeMoveSpeed()
{
	float NewMoveSpeed = bIsSprint ? SprintSpeed : WalkSpeed;

	GetCharacterMovement()->MaxWalkSpeed = NewMoveSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = NewMoveSpeed * CrouchSpeedMultiplier;
}

void ABOCharacter::OnEquipmentItemChanged(EEquipmentSlot Slot, UItemInstanceBase* ItemInstanceBase)
{
	if (!IsValid(EquipmentManagerComponent))
	{
		return;
	}

	const bool bWasActiveSlot = EquipmentManagerComponent->GetActiveSlot() == Slot;

	// 해당 슬롯의 기존 장비 제거
	if (EquipmentManagerComponent->HasEquipment(Slot))
	{
		EquipmentManagerComponent->Unassign(Slot);
	}

	// 슬롯이 비워진 경우 제거만 하고 종료
	if (!IsValid(ItemInstanceBase))
	{
		return;
	}

	// 새 장비를 핸들러에 등록
	if (!EquipmentManagerComponent->Assign(Slot, ItemInstanceBase))
	{
		return;
	}

	// 변경 전 활성 슬롯이었다면 새 장비도 실제 장착
	if (bWasActiveSlot)
	{
		EquipmentManagerComponent->Equip(Slot);
	}
}

void ABOCharacter::OnMenuOpenStateChanged(bool bAnyMenuOpen)
{
	UpdateMovementEnabled();
}

void ABOCharacter::UpdateMovementEnabled()
{
	const bool bIsDead = IsValid(StatComponent) && StatComponent->GetIsDead();

	const UUIManager* UIManager = UUIManager::Get(this);

	const bool bAnyMenuOpen = IsValid(UIManager) && UIManager->IsAnyMenuOpen();

	SetMovementEnabled(!bIsDead && !bAnyMenuOpen);
}

void ABOCharacter::HandleDeath(AActor* DamageCauser)
{
	DeathDamageCauser = DamageCauser;
	bDeathSequenceFinished = false;

	// 사망 상태를 반영하여 이동 차단
	UpdateMovementEnabled();

	if (IsValid(InteractComponent))
	{
		InteractComponent->SetInteractionEnabled(false);
	}

	// 달리기와 조준 해제
	bIsSprint = false;
	bIsAiming = false;

	ChangeMoveSpeed();

	if (IsValid(EquipmentManagerComponent))
	{
		EquipmentManagerComponent->Unequip();
	}

	if (!IsValid(DeathMontage) || !IsValid(GetMesh()))
	{
		FinishPlayerDeath();
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (!IsValid(AnimInstance))
	{
		FinishPlayerDeath();
		return;
	}

	const float MontageDuration = AnimInstance->Montage_Play(DeathMontage);

	if (MontageDuration <= 0.0f)
	{
		FinishPlayerDeath();
		return;
	}

	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ABOCharacter::FinishPlayerDeath, MontageDuration, false);
}

void ABOCharacter::FinishPlayerDeath()
{
	if (bDeathSequenceFinished)
	{
		return;
	}

	UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}

	ABOGameMode* GameMode = World->GetAuthGameMode<ABOGameMode>();

	if (!IsValid(GameMode))
	{
		return;
	}

	bDeathSequenceFinished = true;

	if (AMonsterCharacter* KillerMonster = Cast<AMonsterCharacter>(DeathDamageCauser.Get()))
	{
		GameMode->SetKillerMonster(KillerMonster->GetMonsterID());
	}

	GameMode->EndFarming(EFarmingResult::Fail);
}

void ABOCharacter::TryEquipSlot(EEquipmentSlot Slot)
{
	if (!IsValid(PlayerInventoryComponent) || !IsValid(EquipmentManagerComponent))
	{
		return;
	}

	// 인벤토리 장비 슬롯에 실제 아이템이 있을 때만 장착 요청
	if (!IsValid(PlayerInventoryComponent->GetEquipmentItem(Slot)))
	{
		return;
	}

	EquipmentManagerComponent->Equip(Slot);
}

void ABOCharacter::BindingEquipmentManagerComponentDelegates()
{
	if (!EquipmentManagerComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ABOCharacter] EquipmentManagerComponent의 델리게이트 바인딩 실패 : 유효하지 않은 EquipmentManagerComponent"));
		return;
	}

	// Equipment Changed
	EquipmentManagerComponent->OnActiveSlotChangedDelegate.AddUObject(this, &ABOCharacter::OnActiveSlotChanged);

	// Primary & Secondary (Range Weapon)
	EquipmentManagerComponent->OnFireExecutedDelegate.AddUObject(this, &ABOCharacter::OnFireExecuted);
	EquipmentManagerComponent->CanReloadDelegate.BindUObject(this, &ABOCharacter::CanReload);
	EquipmentManagerComponent->RequestReloadAmmoDelegate.BindUObject(this, &ABOCharacter::RequestReloadAmmo);
}

void ABOCharacter::OnActiveSlotChanged(EEquipmentSlot Slot, UEquippableItemInstance* EquippableItemInstance)
{
	if (!EquippableItemInstance)
	{
		return;
	}

	// 장비 데이터 확인
	const FEquippableItemDataRow* EquippableItemData = EquippableItemInstance->GetEquippableItemData();
	if (!EquippableItemData)
	{
		return;
	}

	// 장비 애니메이션 데이터 확인
	UEquipmentAnimationDataAsset* EquipmentAnimationData = EquippableItemData->EquipmentAnimationData;
	if (!EquipmentAnimationData)
	{
		return;
	}

	// 애니메이션 데이터 적용
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		if (UBOAnimInstance* BOAnimInstance = Cast<UBOAnimInstance>(AnimInstance))
		{
			BOAnimInstance->ApplyEquipmentAnimation(EquipmentAnimationData);
		}
	}
}

void ABOCharacter::OnFireExecuted() const
{
	if (!GetMesh() || !GetMesh()->GetAnimInstance())
	{
		return;
	}

	UBOAnimInstance* AnimInstance = Cast<UBOAnimInstance>(GetMesh()->GetAnimInstance());
	if (!IsValid(AnimInstance))
	{
		return;
	}

	if (bIsAiming)
	{
		AnimInstance->PlayFireAimMontage();
	}
	else
	{
		AnimInstance->PlayFireHipMontage();
	}
}

bool ABOCharacter::CanReload(const FName& AmmoItemID) const
{
	if (!IsValid(PlayerInventoryComponent))
	{
		return false;
	}

	if (AmmoItemID.IsNone())
	{
		return false;
	}

	if (bIsRolling || !bMovementEnabled)
	{
		return false;
	}

	const int32 ItemIndex = PlayerInventoryComponent->FindItemIndex(AmmoItemID);

	if (ItemIndex == INDEX_NONE)
	{
		return false;
	}

	const UItemInstanceBase* AmmoItem = PlayerInventoryComponent->GetItem(ItemIndex);

	return IsValid(AmmoItem) && AmmoItem->GetStackCount() > 0;
}

int32 ABOCharacter::RequestReloadAmmo(const FName& AmmoItemID, const int32 RequestedAmmoCount)
{
	if (!IsValid(PlayerInventoryComponent))
	{
		return 0;
	}

	if (AmmoItemID.IsNone() || RequestedAmmoCount <= 0)
	{
		return 0;
	}

	int32 SuppliedAmmoCount = 0;

	while (SuppliedAmmoCount < RequestedAmmoCount)
	{
		const int32 ItemIndex = PlayerInventoryComponent->FindItemIndex(AmmoItemID);

		if (ItemIndex == INDEX_NONE)
		{
			break;
		}

		UItemInstanceBase* AmmoItem = PlayerInventoryComponent->GetItem(ItemIndex);

		if (!IsValid(AmmoItem))
		{
			break;
		}

		const int32 StackCount = AmmoItem->GetStackCount();

		if (StackCount <= 0)
		{
			break;
		}

		const int32 RemainingRequest = RequestedAmmoCount - SuppliedAmmoCount;

		const int32 ConsumeCount = FMath::Min(StackCount, RemainingRequest);

		if (!PlayerInventoryComponent->RemoveItem(ItemIndex, ConsumeCount))
		{
			break;
		}

		SuppliedAmmoCount += ConsumeCount;
	}

	return SuppliedAmmoCount;
}

FName ABOCharacter::GetRollSectionName() const
{
	if (MoveInput.IsNearlyZero())
	{
		return TEXT("Roll_F");
	}

	const float Angle = FMath::RadiansToDegrees(FMath::Atan2(MoveInput.Y, MoveInput.X));

	// W
	if (Angle >= -22.5f && Angle < 22.5f)
	{
		return TEXT("Roll_F");
	}

	// W + D
	if (Angle >= 22.5f && Angle < 67.5f)
	{
		return TEXT("Roll_FR");
	}

	// D
	if (Angle >= 67.5f && Angle < 112.5f)
	{
		return TEXT("Roll_R");
	}

	// S + D
	if (Angle >= 112.5f && Angle < 157.5f)
	{
		return TEXT("Roll_BR");
	}

	// S
	if (Angle >= 157.5f || Angle < -157.5f)
	{
		return TEXT("Roll_B");
	}

	// S + A
	if (Angle >= -157.5f && Angle < -112.5f)
	{
		return TEXT("Roll_BL");
	}

	// A
	if (Angle >= -112.5f && Angle < -67.5f)
	{
		return TEXT("Roll_L");
	}

	// W + A
	return TEXT("Roll_FL");
}

FVector ABOCharacter::GetRollDirection() const
{
	FVector Direction = GetActorForwardVector() * MoveInput.X + GetActorRightVector() * MoveInput.Y;

	Direction.Z = 0.0f;

	if (Direction.IsNearlyZero())
	{
		Direction = GetActorForwardVector();
		Direction.Z = 0.0f;
	}

	return Direction.GetSafeNormal();
}

void ABOCharacter::OnRollMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage != RollMontage)
	{
		return;
	}

	StopRoll();

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();

	if (IsValid(MovementComponent))
	{
		MovementComponent->Velocity.X = 0.0f;
		MovementComponent->Velocity.Y = 0.0f;
	}
}

void ABOCharacter::OnReloadMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage != RollMontage)
	{
		return;
	}

	StopRoll();

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();

	if (IsValid(MovementComponent))
	{
		MovementComponent->Velocity.X = 0.0f;
		MovementComponent->Velocity.Y = 0.0f;
	}
}

void ABOCharacter::AddTestItem(FName ItemID, int32 Count)
{
	if (!PlayerInventoryComponent)
		return;

	UItemInstanceBase* Item = FItemFactory::CreateItemInstance(this, ItemID, Count);
	if (!Item)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddTestItem: '%s' 아이템을 찾을 수 없습니다. DataTable의 Row Name을 확인하세요."), *ItemID.ToString());
		return;
	}

	if (!PlayerInventoryComponent->AddItem(Item))
	{
		UE_LOG(LogTemp, Warning, TEXT("AddTestItem: 인벤토리에 빈 슬롯이 없습니다."));
	}
}

