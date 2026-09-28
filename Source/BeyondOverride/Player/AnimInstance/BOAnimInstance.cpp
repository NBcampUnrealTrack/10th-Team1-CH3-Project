#include "Player/AnimInstance/BOAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
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

	bool bIsFireMontagePlaying = false;

	if (IsValid(CurrentEquipmentData))
	{
		if (IsValid(CurrentEquipmentData->FireHip))
		{
			bIsFireMontagePlaying |= Montage_IsPlaying(CurrentEquipmentData->FireHip);
		}

		if (IsValid(CurrentEquipmentData->FireAim))
		{
			bIsFireMontagePlaying |= Montage_IsPlaying(CurrentEquipmentData->FireAim);
		}
	}

	bIsShooting = bIsFireMontagePlaying;
	EquipmentGroundSpeed = bIsFireMontagePlaying ? 0.0f : GroundSpeed;

	const FVector LocalVelocity = Character->GetActorTransform().InverseTransformVectorNoScale(Velocity);

	const FRotator AimRotation = Character->GetBaseAimRotation();
	const FRotator ActorRotation = Character->GetActorRotation();
	const FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, ActorRotation);
	AimPitch = FMath::Clamp(DeltaRotation.Pitch, -90.0f, 90.0f);

	Direction = FMath::RadiansToDegrees(FMath::Atan2(LocalVelocity.Y, LocalVelocity.X));
	bIsFalling = MovementComponent->IsFalling();

	const bool bHasAcceleration = !MovementComponent->GetCurrentAcceleration().IsNearlyZero();

	bShouldMove = GroundSpeed > 3.0f && bHasAcceleration;

	bIsCrouch = MovementComponent->IsCrouching();
	bIsAiming = Character->GetIsAiming();

	const bool bApplyAimOffset = bHasAimOffset && (bIsAiming || bIsShooting);
	const float TargetAlpha = bApplyAimOffset ? 1.0f : 0.0f;

	AimOffsetAlpha = FMath::FInterpTo(AimOffsetAlpha, TargetAlpha, DeltaSeconds, 12.0f);

	/*if (!bHasAimOffset && AimOffsetAlpha < 0.01f)
	{
		AimOffsetAlpha = 0.0f;
		EquipmentAimOffset = nullptr;
	}*/
}

void UBOAnimInstance::PlayFireMontage(UAnimMontage* FireMontage)
{
	if (!IsValid(FireMontage))
	{
		return;
	}

	if (LastFireMontage != FireMontage)
	{
		LastFireMontage = FireMontage;
		FireSectionIndex = 0;
	}

	const int32 SectionCount = FireMontage->GetNumSections();
	const float Duration = Montage_Play(FireMontage);

	if (Duration <= 0.0f)
	{
		return;
	}

	if (SectionCount > 1)
	{
		const FName SectionName = FireMontage->GetSectionName(FireSectionIndex);
		Montage_JumpToSection(SectionName, FireMontage);
		FireSectionIndex = (FireSectionIndex + 1) % SectionCount;
	}
}

void UBOAnimInstance::ApplyEquipmentAnimation(const UEquipmentAnimationDataAsset* NewData)
{
	if (!IsValid(NewData))
	{
		return;
	}

	CurrentEquipmentData = NewData;

	EquipmentHipLocomotion = NewData->LocomotionHip;
	EquipmentAimLocomotion = NewData->LocomotionAim;
	EquipmentAimOffset = NewData->AimOffset;
	EquipmentJump = NewData->Jump;
	EquipmentFallingLoop = NewData->FallingLoop;
	EquipmentLand = NewData->Land;

	bHasAimOffset = IsValid(NewData->AimOffset);

	if (bHasAimOffset)
	{
		EquipmentAimOffset = NewData->AimOffset;
	}
	else
	{
		AimOffsetAlpha = 0.0f;
		EquipmentAimOffset = nullptr;
	}
}

float UBOAnimInstance::PlayEquipMontage()
{
	if (!CurrentEquipmentData || !CurrentEquipmentData->Equip)
	{
		return 0.f;
	}

	return Montage_Play(CurrentEquipmentData->Equip);
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

	PlayFireMontage(CurrentEquipmentData->FireHip);

	if (!CurrentEquipmentData->WeaponFire)
	{
		return;
	}

	// Montage_Play(CurrentEquipmentData->WeaponFire);
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

	PlayFireMontage(CurrentEquipmentData->FireAim);

	if (!CurrentEquipmentData->WeaponFire)
	{
		return;
	}

	// Montage_Play(CurrentEquipmentData->WeaponFire);
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

	// Montage_Play(CurrentEquipmentData->WeaponReloadHip);
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

	// Montage_Play(CurrentEquipmentData->WeaponReloadAim);
}

bool UBOAnimInstance::IsReloadMontagePlaying() const
{
	if (!IsValid(CurrentEquipmentData))
	{
		return false;
	}

	if (IsValid(CurrentEquipmentData->ReloadHip) && Montage_IsPlaying(CurrentEquipmentData->ReloadHip))
	{
		return true;
	}

	if (IsValid(CurrentEquipmentData->ReloadAim) && Montage_IsPlaying(CurrentEquipmentData->ReloadAim))
	{
		return true;
	}

	return false;
}
