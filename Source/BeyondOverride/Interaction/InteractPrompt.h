#pragma once

#include "CoreMinimal.h"

#include "InteractPrompt.generated.h"

// 상호작용 물건이 UI 에게 건네는 폼
// 물건은 자기가 어떻게 그려지는지 모르고, UI 는 상대가 창고인지
// 맨홀인지 모른다. 이 구조체만 주고받는다.
USTRUCT(BlueprintType)
struct BEYONDOVERRIDE_API FInteractPrompt
{
	GENERATED_BODY()

	// 물건 이름. "창고", "탈출구", "제작대"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	FText Title;

	// 행동 문구. "[E] 키를 눌러 열기", "[E] 키를 눌러 탈출"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	FText ActionText;

	// 0 이면 즉시 실행. 0 보다 크면 그 시간만큼 눌러야 완료된다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact",
			  meta = (ClampMin = "0.0", UIMax = "10.0"))
	float HoldSeconds = 0.f;

	// false 면 프롬프트가 뜨지만 눌러도 실행되지 않는다.
	// 기본값이 true 인 이유: 언리얼은 프로퍼티 메모리를 0 으로 채우므로
	// 초기값을 안 주면 새로 만든 액터가 조용히 "사용 불가"가 된다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	bool bEnabled = true;

	// bEnabled 가 false 일 때 대신 보여줄 이유.
	// "개방 장치가 작동하지 않았습니다", "가방이 가득 찼습니다"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	FText DisableReason;

	// 홀드 중에 움직이면 취소되는가. 탈출처럼 제자리에 있어야 하는 것에 쓴다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	bool bMoveCancel = false;

	// UInteractComponent 가 "값이 바뀔 때만" UI 에 알리기 위해ㅁ
	// 매 프레임 SetText 를 부르면 Slate 가 매번 다시 그려서 낭비다.
	bool operator==(const FInteractPrompt &Other) const
	{
		return Title.IdenticalTo(Other.Title) &&
			   ActionText.IdenticalTo(Other.ActionText) &&
			   DisableReason.IdenticalTo(Other.DisableReason) &&
			   bEnabled == Other.bEnabled && bMoveCancel == Other.bMoveCancel &&
			   FMath::IsNearlyEqual(HoldSeconds, Other.HoldSeconds);
	}

	bool operator!=(const FInteractPrompt &Other) const
	{
		return !(*this == Other);
	}
};
