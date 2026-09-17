#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "CharacterPreviewComponent.generated.h"

class USceneCaptureComponent2D;
class UPointLightComponent;
class UTextureRenderTarget2D;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BEYONDOVERRIDE_API UCharacterPreviewComponent : public UActorComponent
{
	GENERATED_BODY()

  public:
	UCharacterPreviewComponent();

  protected:
	virtual void OnRegister() override;
	virtual void BeginPlay() override;

  public:
	// 인벤토리 열림/닫힘에 맞춰 호출 - 캡처 시작/정지
	UFUNCTION(BlueprintCallable, Category = "UI|Preview")
	void SetPreviewActive(bool bActive);

	UFUNCTION(BlueprintCallable, Category = "UI|Preview")
	UTextureRenderTarget2D* GetPreviewRenderTarget() const
	{
		return PreviewRenderTarget;
	}

  protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<USceneCaptureComponent2D> PreviewCapture;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UPointLightComponent> PreviewLight;

	// BP_BOCharacter 클래스 디폴트에서 RT 에셋을 꽂아넣을 슬롯
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Preview")
	TObjectPtr<UTextureRenderTarget2D> PreviewRenderTarget;

	FTimerHandle CaptureTimerHandle;

	void CapturePreviewTick();
};
