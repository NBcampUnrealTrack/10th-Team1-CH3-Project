#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Interaction/Internal/InteractableInterface.h"

#include "InteractableActorBase.generated.h"

class UInteractHighlightComponent;

// 상호작용 액터의 공통 베이스
//
// 보관함/탈출구/제작대/상점NPC 가 전부 이걸 상속한다.
//
// 베이스가 대신 해주는 것
//   1. 하이라이트 컴포넌트 부착 + 쳐다볼 때 켜고 끄기
//   2. 상호작용 콜리전 채널 자동 설정 (잊을 수 없게)
//   3. FInteractPrompt 조립 — 하위는 조건만 고치면 된다
//   4. 홀드 완료 시점에 조건 재검사
//
// 새 상호작용 물건 만드는 법
//   C++
//     1) 이 클래스를 상속
//     2) 생성자에서 자기 메시를 만들어 RootComponent 에 붙인다
//     3) PerformInteract() 를 오버라이드 -> 실제로 할 일
//     4) (선택) CanInteract() 오버라이드 -> 조건부로 막고 싶을 때
//
//   블루프린트
//     1) 이 클래스를 부모로 BP 생성
//     2) 메시 컴포넌트 추가
//     3) 이벤트 그래프에서 On Interact Performed 이벤트 사용
UCLASS(Abstract)
class BEYONDOVERRIDE_API AInteractableActorBase : public AActor,
												  public IInteractableInterface
{
	GENERATED_BODY()

  public:
	AInteractableActorBase();

	// IInteractableInterface
	// 탐지기가 부른다. 직접 부를 일은 없다.
	virtual FInteractPrompt
	GetInteractPrompt(AActor *Interactor) const override;

	virtual void OnFocusBegin(AActor *Interactor) override;
	virtual void OnFocusEnd(AActor *Interactor) override;
	virtual void OnInteractComplete(AActor *Interactor) override;

  protected:
	virtual void PostInitializeComponents() override;

	// 하위 클래스가 오버라이드하는 지점
	// 실제로 하는 일: 창고는 UI 열기, 탈출구는 탈출 처리
	virtual void PerformInteract(AActor *Interactor)
	{
	}

	// 지금 상호작용이 가능한지, false 면 프롬프트가 빨간 글씨로 바뀐다.
	// 기본값은 에디터에서 설정한 PromptData 를 그대로 따른다.
	// 상태에 따라 달라지는 물건(탈출구 등)이 이것만 오버라이드하면 된다.
	virtual bool CanInteract(AActor *Interactor, FText &OutReason) const;

	// 화면에 뜰 문구. 개수나 상태를 섞어야 하면 오버라이드한다.
	// 여기서 FText::Format 을 매 프레임 새로 만들면
	// 탐지기의 "값이 바뀔 때만 보내기" 최적화가 무력화된다.
	// 미리 만들어 캐시해 두고 그걸 돌려주기
	virtual FText GetDisplayTitle(AActor *Interactor) const;
	virtual FText GetDisplayAction(AActor *Interactor) const;

	// 블루프린트용
	// 상호작용이 완료되면 불린다. BP 하위 클래스가 여기에 노드를 붙인다.
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction",
			  meta = (DisplayName = "On Interact Performed"))
	void OnInteractPerformed(AActor *Interactor);

	// 공통 데이터
	// 에디터에서 채우는 기본값: 문구/홀드 시간/이동 취소 여부
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	FInteractPrompt PromptData;

	// 쳐다볼 때 외곽을 강조한다. 스태틱이든 스켈레탈이든 알아서 처리
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<UInteractHighlightComponent> Highlight;

	// 끄면 콜리전 채널을 자동 설정하지 않는다.
	// 특정 컴포넌트만 반응하게 손으로 제어하려는 경우에만 쓴다.
	UPROPERTY(EditDefaultsOnly, Category = "Interaction|Advanced")
	bool bAutoSetupInteractCollision = true;
};
