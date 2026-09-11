#include "Player/AnimInstance/BOAnimInstance.h"

#include "Player/Character/BOCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/BlendSpace.h"

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

	//void UBOAnimInstance::ApplyEquipmentAnimation(UEquipmentAnimationData* NewData)
	//{
	//    if (!IsValid(NewData))
	//    {
	//        return;
	//    }
	//
	//    CurrentEquipmentData = NewData;
	//
	//    EquipmentIdle = NewData->Idle;
	//    EquipmentLocomotion = NewData->Locomotion;
	//    EquipmentJumpStart = NewData->JumpStart;
	//    EquipmentJumpLoop = NewData->JumpLoop;
	//    EquipmentJumpLand = NewData->JumpLand;
	//    EquipmentAim = NewData->Aim;
}