#include "Interaction/InteractableActorBase.h"

#include "Components/PrimitiveComponent.h"
#include "Interaction/Internal/InteractHighlightComponent.h"
#include "Interaction/Internal/InteractionChannels.h"

AInteractableActorBase::AInteractableActorBase()
{
	// 플레이어가 쳐다볼 때만 반응하므로 Tick 을 끈다.
	PrimaryActorTick.bCanEverTick = false;

	// 루트는 빈 껍데기.
	// 메시를 여기 넣지 않는 이유:
	// 보관함, 탈출구는 스태틱 메시지만 상점 NPC 는 스켈레탈 메시다.
	// 베이스가 스태틱 메시를 루트로 박으면 NPC 가 들어올 수 없다.
	// 하위 클래스가 자기에게 맞는 메시를 만들어 여기 붙인다.
	RootComponent =
		CreateDefaultSubobject<USceneComponent>(TEXT("InteractRoot"));

	Highlight = CreateDefaultSubobject<UInteractHighlightComponent>(
		TEXT("HighlightComp"));
}

void AInteractableActorBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!bAutoSetupInteractCollision)
	{
		return;
	}

	// 하위 클래스가 만든 메시가 무엇이든 상호작용 채널을 열어준다.
	// 이걸 베이스에서 하는 이유는 하위 클래스마다 SetCollisionResponseToChannel 을 손으로 쓰면
	// 언젠가 하나를 잊는다. 그러면 "광선이 액터를 통과하는데
	// 코드는 멀쩡한" 버그가 되고, 원인을 찾는 데 시간이 든다.
	// 잊을 수 있는 일은 잊을 수 없게 만드는 게 낫다.
	TArray<UPrimitiveComponent *> Prims;
	GetComponents<UPrimitiveComponent>(Prims);

	for (UPrimitiveComponent *P : Prims)
	{
		if (P && P->IsCollisionEnabled())
		{
			P->SetCollisionResponseToChannel(ECC_Interaction, ECR_Block);

			//P->SetCollisionResponseToChannel(ECC_InteractionDetector, ECR_Overlap);

			// 상호작용 탐지용 겹침 이벤트는 무조건 켠다.
			P->SetGenerateOverlapEvents(true);
		}
	}
}

// 프롬프트 조립
FInteractPrompt
AInteractableActorBase::GetInteractPrompt(AActor *Interactor) const
{
	// 에디터에서 설정한 값으로 시작한다.
	// HoldSeconds 와 bMoveCancel 은 그대로 쓰인다.
	FInteractPrompt Prompt = PromptData;

	Prompt.Title = GetDisplayTitle(Interactor);
	Prompt.ActionText = GetDisplayAction(Interactor);

	// 사용 가능 여부는 매번 새로 묻는다.
	// 탈출구가 "잠김" > "탈출 가능" 으로 바뀌면 즉시 반영된다.
	FText Reason;
	const bool bCan = CanInteract(Interactor, Reason);

	Prompt.bEnabled = bCan;
	if (!bCan)
	{
		Prompt.DisableReason = Reason;
	}

	return Prompt;
}

bool AInteractableActorBase::CanInteract(AActor *Interactor,
										 FText &OutReason) const
{
	// 기본 동작, 에디터에서 정한 값을 그대로 따른다.
	OutReason = PromptData.DisableReason;
	return PromptData.bEnabled;
}

FText AInteractableActorBase::GetDisplayTitle(AActor *Interactor) const
{
	return PromptData.Title;
}

FText AInteractableActorBase::GetDisplayAction(AActor *Interactor) const
{
	return PromptData.ActionText;
}

// 포커스
void AInteractableActorBase::OnFocusBegin(AActor *Interactor)
{
	if (Highlight)
	{
		Highlight->SetHighlighted(true);
	}
}

void AInteractableActorBase::OnFocusEnd(AActor *Interactor)
{
	if (Highlight)
	{
		Highlight->SetHighlighted(false);
	}
}

// 실행
void AInteractableActorBase::OnInteractComplete(AActor *Interactor)
{
	// 여기서 한 번 더 검사하는 이유:
	// 누르기 시작할 때는 가능했는데 3초 홀드가 끝나는 사이에
	// 조건이 깨질 수 있다. (가방이 꽉 찼거나, 탈출구가 다시 잠겼거나)
	FText Reason;
	if (!CanInteract(Interactor, Reason))
	{
		return;
	}

	// 공통 처리 자리
	// 효과음이나 이펙트가 생기면 여기 한 곳에 넣으면
	// 모든 상호작용 물건에 한꺼번에 적용된다.
	PerformInteract(Interactor);      // C++ 하위 클래스
	OnInteractPerformed(Interactor);  // BP 하위 클래스
}
