#include "Player/Preview/PreviewCharacterActor.h"

#include "ActorComponents/EquipmentHandlerComponent.h"
#include "ActorComponents/EquipmentManagerComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SkeletalMeshComponent.h"
#include "DataTables/Items/EquippableItemDataRow.h"
#include "Engine/TextureRenderTarget2D.h"
#include "GameFramework/Character.h"
#include "Items/Objects/EquippableItemInstance.h"
#include "Player/AnimInstance/BOAnimInstance.h"

APreviewCharacterActor::APreviewCharacterActor()
{
	PrimaryActorTick.bCanEverTick = false;

	PreviewRoot = CreateDefaultSubobject<USceneComponent>(TEXT("PreviewRoot"));
	RootComponent = PreviewRoot;

	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(PreviewRoot);
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyMesh->SetCastShadow(false);

	PreviewCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("PreviewCapture"));
	PreviewCapture->SetupAttachment(BodyMesh);
	PreviewCapture->SetRelativeLocation(FVector(-20.f, 200.f, 89.f));      
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
	PreviewLight->Intensity = 2000.f;
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

	// 실제 캐릭터가 현재 손에 들고 있는 장비의 애니메이션 데이터(장착 자세용 블렌드스페이스 등)를
	// 프리뷰의 AnimInstance에도 그대로 적용
	UEquipmentManagerComponent* SourceEquipmentManager = SourceCharacter->FindComponentByClass<UEquipmentManagerComponent>();
	UEquipmentHandlerComponent* ActiveHandler = SourceEquipmentManager ? SourceEquipmentManager->GetActiveHandler() : nullptr;
	UEquippableItemInstance* ActiveItemInstance = ActiveHandler ? ActiveHandler->GetEquippableItemInstance() : nullptr;
	const FEquippableItemDataRow* ActiveItemData = ActiveItemInstance ? ActiveItemInstance->GetEquippableItemData() : nullptr;

	if (UBOAnimInstance* PreviewAnimInstance = Cast<UBOAnimInstance>(BodyMesh->GetAnimInstance()))
	{
		if (ActiveItemData && ActiveItemData->EquipmentAnimationData)
		{
			PreviewAnimInstance->ApplyEquipmentAnimation(ActiveItemData->EquipmentAnimationData);
		}
	}
}

void APreviewCharacterActor::PlayEquipAnimation()
{
	if (UBOAnimInstance* PreviewAnimInstance = Cast<UBOAnimInstance>(BodyMesh->GetAnimInstance()))
	{
		PreviewAnimInstance->PlayEquipMontage();
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
