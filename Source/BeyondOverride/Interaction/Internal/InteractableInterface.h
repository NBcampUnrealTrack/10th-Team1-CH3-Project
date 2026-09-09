#pragma once

#include "CoreMinimal.h"

#include "Interaction/InteractPrompt.h"
#include "UObject/Interface.h"

#include "InteractableInterface.generated.h"

// 상호작용 "약속"
//
// E 를 눌러 쓰는 물건은 전부 이걸 구현한다.
// 탐지기(UInteractComponent)는 이 약속만 알면 되고,
// 상대가 창고인지 맨홀인지는 몰라도 된다.
//
// 새 상호작용 물건을 만들 때는 이 인터페이스를 직접 구현하는 대신
// AInteractableActorBase 를 상속 이 인터페이스를 직접 구현해야 하는 경우는
// AActor 를 상속할 수 없는 특수한 물건

UINTERFACE(MinimalAPI)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

class BEYONDOVERRIDE_API IInteractableInterface
{
	GENERATED_BODY()

  public:
	// 지금 이 물건이 뭐라고 표시돼야 하는가
	//
	// 매 프레임 불린다. 그래서 상태에 따라 답이 달라져도 된다.
	// 맨홀이 잠겨 있으면 "사용 불가", 열린 뒤엔 "탈출"
	//
	// Interactor = 쳐다보고 있는 주체(플레이어 캐릭터)
	// 누가 보는지에 따라 답을 바꿀 수도 있다.
	virtual FInteractPrompt GetInteractPrompt(AActor *Interactor) const = 0;

	// 실제로 실행, 여기서 창을 열거나 탈출을 처리한다.
	virtual void OnInteractComplete(AActor *Interactor) = 0;

	// 선택 구현, 필요 없으면 구현X
	// 쳐다보기 시작 / 끝, 하이라이트를 켜고 끄는 자리
	virtual void OnFocusBegin(AActor *Interactor)
	{
	}
	virtual void OnFocusEnd(AActor *Interactor)
	{
	}

	// 홀드 시작 / 중간 취소
	// n초 눌러야 하는 상호작용에서 게이지가 차기 시작할 때와
	// 손을 떼거나 움직여서 취소될 때 불린다.
	virtual void OnInteractStart(AActor *Interactor)
	{
	}
	virtual void OnInteractCancel(AActor *Interactor)
	{
	}
};
