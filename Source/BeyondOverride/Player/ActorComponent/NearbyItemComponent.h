#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NearbyItemComponent.generated.h"

class USphereComponent;
class UPrimitiveComponent;
class UItemInstanceBase;
class AItemPickupBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNearbyItemsChanged, const TArray<AItemPickupBase*>&, NearbyItems);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BEYONDOVERRIDE_API UNearbyItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnNearbyItemsChanged OnNearbyItemsChanged;

	UFUNCTION(BlueprintPure)
	int32 GetItemCount() const;

	UFUNCTION(BlueprintPure)
	bool IsValidSlot(int32 SlotIndex) const;

	UFUNCTION(BlueprintPure)
	AItemPickupBase* GetItemPickup(int32 SlotIndex) const;

	UFUNCTION(BlueprintPure)
	UItemInstanceBase* GetItem(int32 SlotIndex) const;

	UFUNCTION(BlueprintPure)
	TArray<AItemPickupBase*> GetItemPickups() const;

	// 주변 목록 변경
	void AddItemPickup(AItemPickupBase* ItemPickup);
	void RemoveItemPickup(AItemPickupBase* ItemPickup);
	void NotifyItemsChanged();

public:
	UNearbyItemComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION()
	void OnDetectionBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnDetectionEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex
	);

	void BroadcastItemsChanged();

private:
	UPROPERTY()
	TObjectPtr<USphereComponent> DetectionSphere;

	UPROPERTY()
	TArray<TObjectPtr<AItemPickupBase>> NearbyItemPickups;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nearby Item", meta = (AllowPrivateAccess = "true", ClampMin = "0.0"))
	float DetectionRadius = 300.0f;

};
