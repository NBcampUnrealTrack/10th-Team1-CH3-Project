#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "CharacterPreviewComponent.generated.h"

class APreviewCharacterActor;
class UTextureRenderTarget2D;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BEYONDOVERRIDE_API UCharacterPreviewComponent : public UActorComponent
{
	GENERATED_BODY()

  public:
	UCharacterPreviewComponent();

  protected:
	virtual void BeginPlay() override;

  public:
	// 인벤토리 열림/닫힘에 맞춰 호출 - 캡처 시작/정지 + 장비 동기화
	UFUNCTION(BlueprintCallable, Category = "UI|Preview")
	void SetPreviewActive(bool bActive);

	UFUNCTION(BlueprintCallable, Category = "UI|Preview")
	UTextureRenderTarget2D* GetPreviewRenderTarget() const
	{
		return PreviewRenderTarget;
	}

  protected:
	// BP_BOCharacter 클래스 디폴트에서 RT 에셋을 꽂아넣을 슬롯
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Preview")
	TObjectPtr<UTextureRenderTarget2D> PreviewRenderTarget;

	// 조명 격리를 위해 캐릭터와 멀리 떨어진 곳에 스폰되는 프리뷰 전용 무대 액터
	UPROPERTY()
	TObjectPtr<APreviewCharacterActor> PreviewActor;

	// PreviewActor를 스폰할 오프셋 - 월드 어디와도 안 겹치는 먼 위치
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Preview")
	FVector PreviewStageOffset = FVector(0.f, 0.f, 100000.f);
};
