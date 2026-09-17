#include "Player/ActorComponent/CharacterPreviewComponent.h"

#include "Components/PointLightComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

UCharacterPreviewComponent::UCharacterPreviewComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	PreviewCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("PreviewCapture"));
	PreviewCapture->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	PreviewCapture->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	PreviewCapture->ProjectionType = ECameraProjectionMode::Perspective;
	PreviewCapture->bCaptureEveryFrame = false;
	PreviewCapture->bCaptureOnMovement = false;
	PreviewCapture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;

	PreviewLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PreviewLight"));
	PreviewLight->SetupAttachment(PreviewCapture);
	PreviewLight->SetRelativeLocation(FVector(-30.f, 0.f, 30.f));
	PreviewLight->Intensity = 5000.f;
	PreviewLight->AttenuationRadius = 300.f;
	PreviewLight->SetLightColor(FLinearColor::White);
	PreviewLight->CastShadows = false;
}

void UCharacterPreviewComponent::OnRegister()
{
	Super::OnRegister();

	if (AActor* Owner = GetOwner())
	{
		if (USceneComponent* OwnerRoot = Owner->GetRootComponent())
		{
			PreviewCapture->SetupAttachment(OwnerRoot);
		}
	}
}

void UCharacterPreviewComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* Owner = GetOwner())
	{
		PreviewCapture->ShowOnlyActors.Add(Owner); // 이 컴포넌트를 가진 액터(캐릭터) 전체만 캡처
	}

	if (PreviewRenderTarget)
	{
		PreviewCapture->TextureTarget = PreviewRenderTarget;
	}
}

void UCharacterPreviewComponent::SetPreviewActive(bool bActive)
{
	if (PreviewCapture)
	{
		PreviewCapture->bCaptureEveryFrame = false;

		if (bActive)
		{
			PreviewCapture->CaptureScene();

			if (UWorld* World = GetWorld())
			{
				World->GetTimerManager().SetTimer(CaptureTimerHandle, this, &UCharacterPreviewComponent::CapturePreviewTick, 0.05f, true);
			}
		}
		else if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(CaptureTimerHandle);
		}
	}

	if (PreviewLight)
	{
		PreviewLight->SetVisibility(bActive);
	}
}

void UCharacterPreviewComponent::CapturePreviewTick()
{
	if (PreviewCapture)
	{
		PreviewCapture->CaptureScene();
	}
}
