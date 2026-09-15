#include "Player/Character/BOCharacter.h"

#include "EnhancedInputComponent.h"

#include "ActorComponents/EquipmentManagerComponent.h"
#include "Camera/CameraComponent.h"
#include "DataTables/Items/EquippableItemDataRow.h"
#include "Enums/EquipmentSlot.h"
#include "Factory/ItemFactory.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interaction/InteractComponent.h"
#include "Items/Actors/ItemPickupBase.h"
#include "Items/Objects/EquippableItemInstance.h"
#include "Items/Objects/MeleeWeaponInstance.h"
#include "Items/Objects/RangeWeaponInstance.h"
#include "Items/Objects/ThrowableItemInstance.h"
#include "Player/ActorComponent/EquipmentComponent.h"
#include "Player/ActorComponent/InventoryInteractionComponent.h"
#include "Player/ActorComponent/PlayerInventoryComponent.h"
#include "Player/ActorComponent/NearbyItemComponent.h"
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
}

void ABOCharacter::BeginPlay()
{
	Super::BeginPlay();

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
}

void ABOCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid(Camera))
	{
		return;
	}

	const float TargetFOV = bIsAiming ? AimFOV : DefaultFOV;
	const float NewFOV = FMath::FInterpTo(Camera->FieldOfView, TargetFOV, DeltaTime, ZoomSpeed);

	Camera->SetFieldOfView(NewFOV);
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
				EnhancedInput->BindAction(PlayerController->PrimaryAction, ETriggerEvent::Triggered, this, &ABOCharacter::Fire);
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

void ABOCharacter::Move(const FInputActionValue& value)
{
	if (!Controller)
		return;

	if (UUIManager* UIManager = UUIManager::Get(this))
	{
		if (UIManager->IsAnyMenuOpen())
		{
			return;
		}
	}

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

void ABOCharacter::Fire(const FInputActionValue& value)
{
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

void ABOCharacter::Hip(const FInputActionValue& value)
{
	bIsAiming = false;
}

void ABOCharacter::Reload(const FInputActionValue& value)
{
	// 장비 재장전
	if (EquipmentManagerComponent)
	{
		EquipmentManagerComponent->Reload();
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
					//if (ItemInstance->IsA(URangeWeaponInstance::StaticClass()))
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
					//}
					//// Melee Weapon
					//else if (ItemInstance->IsA(UMeleeWeaponInstance::StaticClass()))
					//{
					//	if (EquipmentManagerComponent->Assign(EEquipmentSlot::Melee, ItemInstance))
					//	{
					//		if (PlayerInventoryComponent)
					//		{
					//			PlayerInventoryComponent->SetEquipmentItem(EEquipmentSlot::Melee, ItemInstance);
					//		}
					//		ItemPickup->Destroy();
					//	}
					//}
					////// throwable Weapon
					////else if (ItemInstance->IsA(UMeleeWeaponInstance::StaticClass()))
					////{
					////	if (EquipmentManagerComponent->Assign(EEquipmentSlot::Melee, ItemInstance))
					////	{
					////		ItemPickup->Destroy();
					////	}
					////}
					//else
					//{
					//	if (PlayerInventoryComponent->AddItem(ItemInstance))
					//	{
					//		ItemPickup->Destroy();
					//	}
					//}
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
	//if (EquipmentManagerComponent)
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
	EquipmentManagerComponent->CanReloadDelegate.BindUObject(this, &ABOCharacter::OnCanReload);
	EquipmentManagerComponent->RequestReloadAmmoDelegate.BindUObject(this, &ABOCharacter::OnRequestReloadAmmo);
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
