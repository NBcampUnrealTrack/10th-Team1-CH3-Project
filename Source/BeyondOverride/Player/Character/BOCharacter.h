#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BOCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UEquipmentComponent;
class UStatComponent;
class UInventoryComponent;
class UInteractComponent;

UCLASS()
class BEYONDOVERRIDE_API ABOCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABOCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float WalkSpeed = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed = 600.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float CrouchSpeedMultiplier = 0.5f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SpeedMultiplier = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USkeletalMeshComponent* EquipmentSkeletalMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UEquipmentComponent* EquipmentComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStatComponent* StatComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UInventoryComponent* InventoryComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UInteractComponent* InteractComponent;

private:
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);
	UFUNCTION()
	void ToggleCrouch(const FInputActionValue& value);
	UFUNCTION()
	void Primary(const FInputActionValue& value);
	UFUNCTION()
	void Secondary(const FInputActionValue& value);
	UFUNCTION()
	void InteractPress(const FInputActionValue& value);
	UFUNCTION()
	void InteractRelease(const FInputActionValue& value);
	UFUNCTION()
	void Inventory(const FInputActionValue& value);
	UFUNCTION()
	void Escape(const FInputActionValue& value);

	void ChangeMoveSpeed();

	bool bIsSprint = false;

};
