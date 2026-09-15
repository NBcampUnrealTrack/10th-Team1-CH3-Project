#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "Interaction/InteractPrompt.h"

#include "InteractComponent.generated.h"

class UCapsuleComponent;

// 밖으로 나가는 신호
//
// DYNAMIC 인 이유: 블루프린트에서도 붙일 수 있어야 한다.
// UI 를 C++ 로 만들든 위젯 블루프린트로 만들든 상관없게 된다.
//
// 쳐다보는 대상이 생기거나 사라지거나, 표시 내용이 바뀔 때
// bHasTarget == false 면 Prompt 는 빈 값이다 (UI 를 숨기라는 뜻)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractPromptChanged, bool,
	bHasTarget, FInteractPrompt,
	Prompt);

// 홀드 게이지. 0.0 ~ 1.0. 취소·완료 시 0 이 한 번 더 온다.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractHoldProgress, float,
	Progress);

// 상호작용 탐지기: 플레이어 캐릭터에 붙인다
//
// 하는 일
//   1. 매 프레임 카메라에서 앞으로 광선을 쏜다
//   2. 맞은 게 IInteractableInterface 면 "지금 보는 것"으로 기억한다
//   3. 표시 내용이 바뀌면 OnPromptChanged 로 알린다
//   4. PressInteract / ReleaseInteract 를 받아 홀드를 처리한다
//
// 붙이는 방법
// 캐릭터 블루프린트에서 Add Component → Interact
// (또는 C++ 생성자에서 CreateDefaultSubobject)
//
// 입력 연결
//   IA_Interact · Started   → InteractComp->PressInteract()
//   IA_Interact · Completed → InteractComp->ReleaseInteract()
//   Triggered 로 묶으면 누르고 있는 동안 매 프레임 불려서 망가진다
//
// UI 연결 (UI 담당)
//   InteractComp->OnPromptChanged.AddDynamic(Widget,
//   &U...::HandlePromptChanged);
//   InteractComp->OnHoldProgress.AddDynamic(Widget, &U...::HandleHoldProgress);
//
// 창이 열려 있을 때 (UI 담당)
//   SetInteractionEnabled(false) 를 부르면 탐지가 멈추고 프롬프트가 꺼진다.
//   인벤토리 · 상점 같은 전체 화면 창을 열 때 반드시 호출할 것.

UCLASS(ClassGroup = (Interaction), meta = (BlueprintSpawnableComponent))
class BEYONDOVERRIDE_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractComponent();

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInteractPromptChanged OnPromptChanged;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInteractHoldProgress OnHoldProgress;

	// 캐릭터의 입력에서
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void PressInteract();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void ReleaseInteract();

	// 탐지를 켜고 끈다. 끄면 광선도 안 쏘고 프롬프트도 꺼진다.
	// 전체 화면 UI 를 열 때, 컷신, 사망 시에
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetInteractionEnabled(bool bEnabled);

	// 상태 조회
	UFUNCTION(BlueprintPure, Category = "Interaction")
	bool IsInteractionEnabled() const
	{
		return bInteractionEnabled;
	}

	// 지금 쳐다보는 대상, 없으면 nullptr
	UFUNCTION(BlueprintPure, Category = "Interaction")
	AActor* GetFocusedActor() const
	{
		return FocusedActor.Get();
	}

	// 홀드 진행도 0.0 ~ 1.0, 홀드 중이 아니면 0
	UFUNCTION(BlueprintPure, Category = "Interaction")
	float GetHoldProgress() const;

	UFUNCTION(BlueprintPure, Category = "Interaction")
	bool IsHolding() const
	{
		return bHolding;
	}

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void
		TickComponent(float DeltaTime, ELevelTick TickType,
			FActorComponentTickFunction* ThisTickFunction) override;

	// 광선이 닿는 최대 거리 (cm) 250 = 대략 두 걸음
	// 카메라 거리는 자동 보정돼 3인칭이어도 이 값은 "캐릭터 기준" 거리다
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction",
		meta = (ClampMin = "50.0", UIMax = "500.0"))
	float TraceDistance = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (ClampMin = "0.0", UIMax = "50.0"))
	float TraceRadius = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (ClampMin = "50.0"))
	float DetectionRadius = 300.0f;

	// 홀드 중 이 거리를 넘게 움직이면 취소된다 (bMoveCancel 인 물건만)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction",
		meta = (ClampMin = "0.0"))
	float MoveCancelDistance = 50.f;

	// 켜면 광선이 화면에 그려진다. (개발 중에만)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Debug")
	bool bDrawDebug = true;

private:
	// 쳐다보는 대상을 바꾼다. 이전 것 끄고 새 것 켜는 처리가 들어 있다.
	void SetFocus(AActor* NewTarget);

	// 광선을 쏘고 유효한 상호작용 대상을 돌려준다. (없으면 nullptr)
	AActor* TraceForTarget(FVector& OutViewLoc, FVector& OutTraceEnd,
		bool& bOutHitSomething, FVector& OutHitPoint) const;

	// 프롬프트를 UI 에 보낸다. 값이 바뀌었을 때만 실제로 쏜다.
	void PushPrompt();

	// 프롬프트를 끄라고 알린다. 이미 꺼져 있으면 아무것도 안 한다.
	void ClearPrompt();

	void UpdateHold(float DeltaTime);
	void CancelHold();
	void CompleteHold();

	UPROPERTY()
	TObjectPtr<UCapsuleComponent> DetectionCollision;

	//UFUNCTION()
	//void OnDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//UFUNCTION()
	//void OnDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	//						   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	int32 NearbyInteractableCount = 0;

	// 지금 쳐다보는 대상
	// TWeakObjectPtr 인 이유: 그냥 AActor* 로 들고 있으면 그 액터가 파괴됐을 때(아이템을 주워서
	// 사라지는 경우) 쓰레기 주소가 남아 크래시난다.
	// 약한 참조는 파괴되면 자동으로 null 이 된다.
	TWeakObjectPtr<AActor> FocusedActor;

	// 마지막으로 UI 에 보낸 값. 이것과 다를 때만 새로 보낸다.
	FInteractPrompt LastSentPrompt;

	// UI 에 지금 프롬프트가 떠 있는가.
	bool bPromptVisible = false;

	bool bInteractionEnabled = true;

	// 홀드 시작 시점의 값을 복사해 둔다.
	// 도중에 GetInteractPrompt() 의 답이 바뀌어도 진행도가 튀지 않게 하려는 것
	bool bHolding = false;
	float HoldElapsed = 0.f;
	float HoldDuration = 0.f;
	bool bHoldMoveCancel = false;
	FVector HoldStartLocation = FVector::ZeroVector;
};
