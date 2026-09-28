#include "Player/ActorComponent/CharacterPreviewComponent.h"

#include "ActorComponents/EquipmentManagerComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "GameFramework/Character.h"
#include "Player/Character/BOCharacter.h"
#include "Player/Preview/PreviewCharacterActor.h"

UCharacterPreviewComponent::UCharacterPreviewComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterPreviewComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter)
	{
		return;
	}

	const FVector SpawnLocation = PreviewStageOffset;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	PreviewActor = GetWorld()->SpawnActor<APreviewCharacterActor>(APreviewCharacterActor::StaticClass(), SpawnLocation, FRotator::ZeroRotator, SpawnParams);

	if (PreviewActor)
	{
		PreviewActor->InitializeFromCharacter(OwnerCharacter);
		PreviewActor->SetRenderTarget(PreviewRenderTarget);
	}

	// 인벤토리가 열려있는 동안에도 장비를 바꾸면(장착/해제) 프리뷰가 바로 반영되도록 구독
	if (ABOCharacter* BOOwnerCharacter = Cast<ABOCharacter>(OwnerCharacter))
	{
		if (UEquipmentManagerComponent* EquipmentManager = BOOwnerCharacter->GetEquipmentComponent())
		{
			EquipmentManager->OnActiveSlotChangedDelegate.AddUObject(this, &UCharacterPreviewComponent::OnEquipmentChanged);
		}
	}
}

void UCharacterPreviewComponent::OnEquipmentChanged(EEquipmentSlot Slot, UEquippableItemInstance* EquippableItemInstance)
{
	if (!PreviewActor)
	{
		return;
	}

	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		PreviewActor->SyncEquipmentFrom(OwnerCharacter);
		PreviewActor->PlayEquipAnimation();
	}
}

void UCharacterPreviewComponent::SetPreviewActive(bool bActive)
{
	if (!PreviewActor)
	{
		return;
	}

	if (bActive)
	{
		if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
		{
			PreviewActor->SyncEquipmentFrom(OwnerCharacter);
		}
	}

	PreviewActor->SetActive(bActive);
}
