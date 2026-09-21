#include "Player/ActorComponent/CharacterPreviewComponent.h"

#include "Engine/TextureRenderTarget2D.h"
#include "GameFramework/Character.h"
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

	const FVector SpawnLocation = OwnerCharacter->GetActorLocation() + PreviewStageOffset;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	PreviewActor = GetWorld()->SpawnActor<APreviewCharacterActor>(APreviewCharacterActor::StaticClass(), SpawnLocation, FRotator::ZeroRotator, SpawnParams);

	if (PreviewActor)
	{
		PreviewActor->InitializeFromCharacter(OwnerCharacter);
		PreviewActor->SetRenderTarget(PreviewRenderTarget);
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
