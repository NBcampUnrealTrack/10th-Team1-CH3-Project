#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "InteractHighlightComponent.generated.h"

class UMaterialInterface;

// 쳐다볼 때 외곽을 강조하는 컴포넌트
// 액터에 붙여두고 SetHighlighted(true/false) 호출
// 액터 안의 모든 메시에 한꺼번에 적용된다.
//
// SetOverlayMaterial 로 메시 위에 반투명 머티리얼을 한 겹 덮는다.
// 기존 머티리얼을 건드리지 않으므로 껐을 때 원상복구가 확실하다.
//
// AInteractableActorBase 를 상속하면 이미 붙어 있으므로
// 직접 추가할 필요는 없다.
//
// [추후 시간 남으면 테두리만 빛나도록 수정]
UCLASS(ClassGroup = (Interaction), meta = (BlueprintSpawnableComponent))
class BEYONDOVERRIDE_API UInteractHighlightComponent : public UActorComponent
{
	GENERATED_BODY()

  public:
	UInteractHighlightComponent();

	// 강조를 켜고 끈다.
	UFUNCTION(BlueprintCallable, Category = "Interaction|Highlight")
	void SetHighlighted(bool bOn);

	UFUNCTION(BlueprintPure, Category = "Interaction|Highlight")
	bool IsHighlighted() const
	{
		return bHighlighted;
	}

  protected:
	// 덮어씌울 머티리얼. 에디터에서 물건마다 다르게 줄 수도 있다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Highlight")
	TObjectPtr<UMaterialInterface> HighlightMaterial;

  private:
	bool bHighlighted = false;
};
