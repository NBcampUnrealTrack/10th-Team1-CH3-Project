#include "Player/AnimInstance/BOAnimInstance.h"

#include "Animation/AnimSequenceBase.h"
#include "Animation/BlendSpace.h"
#include "DataAssets/EquipmentAnimationDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/Character/BOCharacter.h"

void UBOAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ABOCharacter>(TryGetPawnOwner());
}

void UBOAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!IsValid(Character))
	{
		return;
	}

	UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();

	if (!IsValid(MovementComponent))
	{
		return;
	}

	Velocity = Character->GetVelocity();

	GroundSpeed = Velocity.Size2D();

	const FVector LocalVelocity = Character->GetActorTransform().InverseTransformVectorNoScale(Velocity);

	Direction = FMath::RadiansToDegrees(FMath::Atan2(LocalVelocity.Y, LocalVelocity.X));
	bIsFalling = MovementComponent->IsFalling();

	const bool bHasAcceleration = !MovementComponent->GetCurrentAcceleration().IsNearlyZero();

	bShouldMove = GroundSpeed > 3.0f && bHasAcceleration;

	bIsCrouch = MovementComponent->IsCrouching();

	ABOCharacter* BOCharacter = Cast<ABOCharacter>(Character);
	if (IsValid(BOCharacter))
	{
		return;
	}

	bIsAiming = BOCharacter->GetIsAiming();
}

void UBOAnimInstance::ApplyEquipmentAnimation(const UEquipmentAnimationDataAsset* NewData)
{
	if (!IsValid(NewData))
	{
		return;
	}

	CurrentEquipmentData = NewData;

	EquipmentHipIdle = NewData->IdleHip;
	EquipmentAimIdle = NewData->IdleAim;
	EquipmentHipLocomotion = NewData->LocomotionHip;
	EquipmentAimLocomotion = NewData->LocomotionAim;
	EquipmentJump = NewData->Jump;
	EquipmentFallingLoop = NewData->FallingLoop;
	EquipmentLand = NewData->Land;
}

void UBOAnimInstance::PlayEquipMontage()
{
	if (!CurrentEquipmentData || !CurrentEquipmentData->Equip)
	{
		return;
	}

	Montage_Play(CurrentEquipmentData->Equip);
}

void UBOAnimInstance::PlayFireHipMontage()
{
	if (!CurrentEquipmentData)
	{
		return;
	}

	if (!CurrentEquipmentData->FireHip)
	{
		return;
	}

	Montage_Play(CurrentEquipmentData->FireHip);

	if (!CurrentEquipmentData->WeaponFire)
	{
		return;
	}

	Montage_Play(CurrentEquipmentData->WeaponFire);
}

void UBOAnimInstance::PlayFireAimMontage()
{
	if (!CurrentEquipmentData)
	{
		return;
	}

	if (!CurrentEquipmentData->FireAim)
	{
		return;
	}

	Montage_Play(CurrentEquipmentData->FireAim);

	if (!CurrentEquipmentData->WeaponFire)
	{
		return;
	}

	Montage_Play(CurrentEquipmentData->WeaponFire);
}

void UBOAnimInstance::PlayReloadHipMontage()
{
	if (!CurrentEquipmentData)
	{
		return;
	}

	if (!CurrentEquipmentData->ReloadHip)
	{
		return;
	}

	Montage_Play(CurrentEquipmentData->ReloadHip);

	if (!CurrentEquipmentData->WeaponReloadHip)
	{
		return;
	}

	Montage_Play(CurrentEquipmentData->WeaponReloadHip);
}

void UBOAnimInstance::PlayReloadAimMontage()
{
	if (!CurrentEquipmentData)
	{
		return;
	}

	if (!CurrentEquipmentData->ReloadAim)
	{
		return;
	}

	Montage_Play(CurrentEquipmentData->ReloadAim);

	if (!CurrentEquipmentData->WeaponReloadAim)
	{
		return;
	}

	Montage_Play(CurrentEquipmentData->WeaponReloadAim);
}
