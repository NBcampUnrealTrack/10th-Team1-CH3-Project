#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BOAnimInstance.generated.h"

class UEquipmentAnimationData;
class ABOCharacter;
class UAnimSequenceBase;
class UBlendSpace;

UCLASS()
class BEYONDOVERRIDE_API UBOAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	//void ApplyEquipmentAnimation(const UEquipmentAnimationData* NewData);

protected:
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Character")
	TObjectPtr<ABOCharacter> Character;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Movement")
	FVector Velocity = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Movement")
	float GroundSpeed = 0.0f;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Movement")
	float Direction = 0.0f;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Movement")
	bool bShouldMove = false;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Movement")
	bool bIsFalling = false;
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Movement")
	bool bIsCrouch = false;

	//UPROPERTY(BlueprintReadOnly, Category = "Equipment")
	//UAnimSequenceBase* EquipmentIdle = nullptr;
	//UPROPERTY(BlueprintReadOnly, Category = "Equipment")
	//UBlendSpace* EquipmentLocomotion = nullptr;
	//UPROPERTY(BlueprintReadOnly, Category = "Equipment")
	//UAnimSequenceBase* EquipmentJumpStart = nullptr;
	//UPROPERTY(BlueprintReadOnly, Category = "Equipment")
	//UAnimSequenceBase* EquipmentJumpLoop = nullptr;
	//UPROPERTY(BlueprintReadOnly, Category = "Equipment")
	//UAnimSequenceBase* EquipmentJumpLand = nullptr;
	//UPROPERTY(BlueprintReadOnly, Category = "Equipment")
	//UAnimSequenceBase* EquipmentAim = nullptr;

	//UPROPERTY(BlueprintReadOnly, Category = "Equipment")
	//UEquipmentAnimationData* CurrentEquipmentData = nullptr;
};
