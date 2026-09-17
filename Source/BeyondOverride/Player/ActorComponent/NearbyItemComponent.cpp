#include "Player/ActorComponent/NearbyItemComponent.h"

#include "Components/SphereComponent.h"
#include "Items/Actors/ItemPickupBase.h"
#include "Items/Objects/ItemInstanceBase.h"

UNearbyItemComponent::UNearbyItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNearbyItemComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();

	if (!IsValid(Owner))
	{
		return;
	}

	USceneComponent* RootComponent = Owner->GetRootComponent();

	if (!IsValid(RootComponent))
	{
		return;
	}

	DetectionSphere = NewObject<USphereComponent>(Owner, TEXT("NearbyItemDetectionSphere"));

	if (!IsValid(DetectionSphere))
	{
		return;
	}

	Owner->AddInstanceComponent(DetectionSphere);

	DetectionSphere->SetupAttachment(RootComponent);
	DetectionSphere->SetSphereRadius(DetectionRadius);
	DetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DetectionSphere->SetGenerateOverlapEvents(true);
	DetectionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	DetectionSphere->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &UNearbyItemComponent::OnDetectionBeginOverlap);
	DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &UNearbyItemComponent::OnDetectionEndOverlap);
	DetectionSphere->RegisterComponent();
}

void UNearbyItemComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(DetectionSphere))
	{
		DetectionSphere->OnComponentBeginOverlap.RemoveDynamic(this, &UNearbyItemComponent::OnDetectionBeginOverlap);
		DetectionSphere->OnComponentEndOverlap.RemoveDynamic(this, &UNearbyItemComponent::OnDetectionEndOverlap);
		DetectionSphere->DestroyComponent();
		DetectionSphere = nullptr;
	}

	NearbyItemPickups.Empty();

	Super::EndPlay(EndPlayReason);
}

void UNearbyItemComponent::OnDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AddItemPickup(Cast<AItemPickupBase>(OtherActor));
}

void UNearbyItemComponent::OnDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	RemoveItemPickup(Cast<AItemPickupBase>(OtherActor));
}

void UNearbyItemComponent::BroadcastItemsChanged()
{
	OnNearbyItemsChanged.Broadcast(GetItemPickups());
}

int32 UNearbyItemComponent::GetItemCount() const
{
	return NearbyItemPickups.Num();
}

bool UNearbyItemComponent::IsValidSlot(int32 SlotIndex) const
{
	return NearbyItemPickups.IsValidIndex(SlotIndex);
}

AItemPickupBase* UNearbyItemComponent::GetItemPickup(int32 SlotIndex) const
{
	if (!NearbyItemPickups.IsValidIndex(SlotIndex))
	{
		return nullptr;
	}

	AItemPickupBase* ItemPickup = NearbyItemPickups[SlotIndex];

	if (!IsValid(ItemPickup))
	{
		return nullptr;
	}

	return ItemPickup;
}

UItemInstanceBase* UNearbyItemComponent::GetItem(int32 SlotIndex) const
{
	AItemPickupBase* ItemPickup = GetItemPickup(SlotIndex);

	if (!IsValid(ItemPickup))
	{
		return nullptr;
	}

	return ItemPickup->GetItemInstance();
}

TArray<AItemPickupBase*> UNearbyItemComponent::GetItemPickups() const
{
	TArray<AItemPickupBase*> Result;

	for (AItemPickupBase* ItemPickup : NearbyItemPickups)
	{
		if (IsValid(ItemPickup))
		{
			Result.Add(ItemPickup);
		}
	}

	return Result;
}

void UNearbyItemComponent::AddItemPickup(AItemPickupBase* ItemPickup)
{
	if (!IsValid(ItemPickup))
	{
		return;
	}

	if (!IsValid(ItemPickup->GetItemInstance()))
	{
		return;
	}

	if (NearbyItemPickups.Contains(ItemPickup))
	{
		return;
	}

	NearbyItemPickups.Add(ItemPickup);

	BroadcastItemsChanged();
}

void UNearbyItemComponent::RemoveItemPickup(AItemPickupBase* ItemPickup)
{
	if (!ItemPickup)
	{
		return;
	}

	if (NearbyItemPickups.Remove(ItemPickup) <= 0)
	{
		return;
	}

	BroadcastItemsChanged();
}

void UNearbyItemComponent::NotifyItemsChanged()
{
	NearbyItemPickups.RemoveAll(
		[](const TObjectPtr<AItemPickupBase>& ItemPickup)
		{
			return !IsValid(ItemPickup);
		}
	);

	BroadcastItemsChanged();
}