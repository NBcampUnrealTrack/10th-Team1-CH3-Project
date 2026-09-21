#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "PreviewCharacterActor.generated.h"

class USceneCaptureComponent2D;
class UPointLightComponent;
class UTextureRenderTarget2D;
class USkeletalMeshComponent;
class ACharacter;

UCLASS()
class BEYONDOVERRIDE_API APreviewCharacterActor : public AActor
{
	GENERATED_BODY()

  public:
	APreviewCharacterActor();

  protected:
	virtual void BeginPlay() override;

  public:
	// 실제 캐릭터의 베이스 메시/애니메이션 블루프린트를 복사 (최초 1회만 호출하면 됨)
	void InitializeFromCharacter(ACharacter* SourceCharacter);

	// 실제 캐릭터에 장착된 장비 메시들을 프리뷰 액터에도 반영 (인벤토리 열 때마다 호출)
	void SyncEquipmentFrom(ACharacter* SourceCharacter);

	void SetRenderTarget(UTextureRenderTarget2D* InRenderTarget);

	// 인벤토리 열림/닫힘에 맞춰 캡처 시작/정지
	void SetActive(bool bActive);

  protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<USkeletalMeshComponent> BodyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<USceneCaptureComponent2D> PreviewCapture;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UPointLightComponent> PreviewLight;

	// 실제 캐릭터의 장비 개수만큼 동적으로 생성되는 프리뷰용 장비 메시들
	UPROPERTY()
	TArray<TObjectPtr<USkeletalMeshComponent>> EquipmentPreviewMeshes;

	FTimerHandle CaptureTimerHandle;

	void CapturePreviewTick();

	USkeletalMeshComponent* GetOrCreateEquipmentPreviewMesh(int32 Index);
};
