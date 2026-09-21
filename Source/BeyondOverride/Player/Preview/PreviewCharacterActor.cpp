#include "Player/Preview/PreviewCharacterActor.h"

#include "ActorComponents/EquipmentHandlerComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "GameFramework/Character.h"

APreviewCharacterActor::APreviewCharacterActor()
{
	PrimaryActorTick.bCanEverTick = false;

	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	RootComponent = BodyMesh;
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyMesh->SetCastShadow(false);

	PreviewCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("PreviewCapture"));
	PreviewCapture->SetupAttachment(BodyMesh);
	PreviewCapture->SetRelativeLocation(FVector(-10.f, 96.f, 89.f));      
	PreviewCapture->SetRelativeRotation(FRotator(0.f, -84.f, 0.f));       
	PreviewCapture->ProjectionType = ECameraProjectionMode::Orthographic; 
	PreviewCapture->OrthoWidth = 100.f;
	PreviewCapture->bCaptureEveryFrame = false;
	PreviewCapture->bCaptureOnMovement = false;
	PreviewCapture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;

	PreviewCapture->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR; 

	FEngineShowFlagsSetting AtmosphereSetting;
	AtmosphereSetting.ShowFlagName = TEXT("Atmosphere");
	AtmosphereSetting.Enabled = false;
	PreviewCapture->ShowFlagSettings.Add(AtmosphereSetting);

	FEngineShowFlagsSetting FogSetting;
	FogSetting.ShowFlagName = TEXT("Fog");
	FogSetting.Enabled = false;
	PreviewCapture->ShowFlagSettings.Add(FogSetting);

	FEngineShowFlagsSetting EyeAdaptationSetting;
	EyeAdaptationSetting.ShowFlagName = TEXT("EyeAdaptation");
	EyeAdaptationSetting.Enabled = false;
	PreviewCapture->ShowFlagSettings.Add(EyeAdaptationSetting);

	PreviewLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PreviewLight"));
	PreviewLight->SetupAttachment(PreviewCapture);
	PreviewLight->SetRelativeLocation(FVector(-30.f, 60.f, 30.f));
	PreviewLight->Intensity = 5000.f;
	PreviewLight->AttenuationRadius = 400.f;
	PreviewLight->SetLightColor(FLinearColor::White);
	PreviewLight->CastShadows = false;
}

void APreviewCharacterActor::BeginPlay()
{
	Super::BeginPlay();

	PreviewCapture->ShowOnlyActors.Add(this); 
	SetActorEnableCollision(false);
}

void APreviewCharacterActor::InitializeFromCharacter(ACharacter* SourceCharacter)
{
	if (!SourceCharacter || !SourceCharacter->GetMesh())
	{
		return;
	}

	USkeletalMeshComponent* SourceMesh = SourceCharacter->GetMesh();

	BodyMesh->SetSkeletalMesh(SourceMesh->GetSkeletalMeshAsset());

	if (SourceMesh->GetAnimInstance())
	{
		BodyMesh->SetAnimInstanceClass(SourceMesh->GetAnimInstance()->GetClass());
	}

	BodyMesh->SetRelativeTransform(SourceMesh->GetRelativeTransform());
}

void APreviewCharacterActor::SyncEquipmentFrom(ACharacter* SourceCharacter)
{
	if (!SourceCharacter)
	{
		return;
	}

	TArray<UEquipmentHandlerComponent*> SourceHandlers;
	SourceCharacter->GetComponents<UEquipmentHandlerComponent>(SourceHandlers);

	for (int32 Index = 0; Index < SourceHandlers.Num(); ++Index)
	{
		UEquipmentHandlerComponent* SourceHandler = SourceHandlers[Index];
		USkeletalMeshComponent* SourceEquipMesh = SourceHandler ? SourceHandler->GetEquipMeshComponent() : nullptr;

		USkeletalMeshComponent* PreviewEquipMesh = GetOrCreateEquipmentPreviewMesh(Index);

		if (!SourceEquipMesh || !SourceEquipMesh->GetSkeletalMeshAsset())
		{
			PreviewEquipMesh->SetSkeletalMesh(nullptr); 
			continue;
		}

		PreviewEquipMesh->SetSkeletalMesh(SourceEquipMesh->GetSkeletalMeshAsset());

		const FName SocketName = SourceEquipMesh->GetAttachSocketName();
		if (BodyMesh->DoesSocketExist(SocketName))
		{
			PreviewEquipMesh->AttachToComponent(BodyMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
		}
	}
}

USkeletalMeshComponent* APreviewCharacterActor::GetOrCreateEquipmentPreviewMesh(int32 Index)
{
	while (EquipmentPreviewMeshes.Num() <= Index)
	{
		const FName ComponentName = *FString::Printf(TEXT("EquipmentPreviewMesh_%d"), EquipmentPreviewMeshes.Num());
		USkeletalMeshComponent* NewMesh = NewObject<USkeletalMeshComponent>(this, ComponentName);
		NewMesh->SetupAttachment(BodyMesh);
		NewMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		NewMesh->SetCastShadow(false);
		NewMesh->RegisterComponent();

		EquipmentPreviewMeshes.Add(NewMesh);
	}

	return EquipmentPreviewMeshes[Index];
}

void APreviewCharacterActor::SetRenderTarget(UTextureRenderTarget2D* InRenderTarget)
{
	if (PreviewCapture)
	{
		PreviewCapture->TextureTarget = InRenderTarget;
	}
}

void APreviewCharacterActor::SetActive(bool bActive)
{
	if (PreviewCapture)
	{
		if (bActive)
		{
			PreviewCapture->CaptureScene();

			GetWorld()->GetTimerManager().SetTimer(CaptureTimerHandle, this, &APreviewCharacterActor::CapturePreviewTick, 0.05f, true);
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(CaptureTimerHandle);
		}
	}

	if (PreviewLight)
	{
		PreviewLight->SetVisibility(bActive);
	}
}

void APreviewCharacterActor::CapturePreviewTick()
{
	if (PreviewCapture)
	{
		PreviewCapture->CaptureScene();
	}
}
