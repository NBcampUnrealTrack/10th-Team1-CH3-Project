#pragma once

#include "CoreMinimal.h"

#include "Animation/AnimInstance.h"

#include "BOAnimInstance.generated.h"

class UEquipmentAnimationDataAsset;
class ABOCharacter;
class UAnimSequenceBase;
class UBlendSpace;

UCLASS()
class BEYONDOVERRIDE_API UBOAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	void ApplyEquipmentAnimation(const UEquipmentAnimationDataAsset* NewData);
	void PlayEquipMontage();
	void PlayFireHipMontage();
	void PlayFireAimMontage();
	void PlayReloadHipMontage();
	void PlayReloadAimMontage();

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Character")
	TObjectPtr<ABOCharacter> Character;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Movement")
	FVector Velocity = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Movement")
	float GroundSpeed = 0.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float EquipmentGroundSpeed = 0.0f;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Movement")
	float Direction = 0.0f;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Movement")
	bool bShouldMove = false;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Movement")
	bool bIsFalling = false;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Movement")
	bool bIsCrouch = false;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Movement")
	bool bIsAiming = false;
	UPROPERTY(BlueprintReadOnly, Category = "Aim")
	float AimPitch = 0.0f;

	//UPROPERTY(BlueprintReadOnly, Category = "Equipment")
	//UAnimSequenceBase* EquipmentHipIdle = nullptr;
	//UPROPERTY(BlueprintReadOnly, Category = "Equipment")
	//UAnimSequenceBase* EquipmentAimIdle = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Equipment")
	UBlendSpace* EquipmentHipLocomotion = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Equipment")
	UBlendSpace* EquipmentAimLocomotion = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Equipment")
	UAnimSequenceBase* EquipmentJump = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Equipment")
	UAnimSequenceBase* EquipmentFallingLoop = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Equipment")
	UAnimSequenceBase* EquipmentLand = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Equipment")
	const UEquipmentAnimationDataAsset* CurrentEquipmentData = nullptr;
};
