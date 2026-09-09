#pragma once

#include "CoreMinimal.h"

#include "Interaction/InteractableActorBase.h"

#include "ExitActor.generated.h"

class AExitActor;

// 탈출이 요청됐을 때 쏘는 신호.
// 이 액터는 게임 흐름을 모른다. 신호만 쏘고, 누가 듣는지 신경 안 쓴다.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnExtractRequested,
											 AExitActor *, ExitPoint,
											 AActor *, Interactor);

// 탈출구 (맨홀)
//   밖 > 나 :  누군가 SetExtractAvailable(true) 를 불러줘야 열린다.
//              그때까지는 잠겨 있다.
//
//   나 > 밖 :  플레이어가 꾹 눌러 탈출하면 OnExtractRequested 를 쏜다.
//             게임 흐름 담당이 듣고 레이드를 종료한다.
//
//   잠김 > PromptData.DisableReason
//   열림 > PromptData.ActionText  ("[E] 꾹 눌러 탈출")
//
//   게임 흐름 담당에서, 시작할 때 한 번,
//     Exit->OnExtractRequested.AddDynamic(this, &A...::HandleExtract);
//
//   개방 조건을 달성했을 때,
//     Exit->SetExtractAvailable(true);
//
//  예정
//   개방 장치(버튼)를 눌러 일정 시간 뒤 열리는 방식으로 바뀔 예정이다.
//   그때는 개방 장치가 SetExtractAvailable 을 대신 불러주므로
//   게임 흐름에서는 구독만 하면 된다.
UCLASS()
class BEYONDOVERRIDE_API AExitActor : public AInteractableActorBase
{
	GENERATED_BODY()

  public:
	AExitActor();

	// 탈출구를 열거나 닫는다.
	// 프롬프트는 매 프레임 새로 조회되므로 즉시 화면에 반영된다.
	UFUNCTION(BlueprintCallable, Category = "Extraction")
	void SetExtractAvailable(bool bAvailable);

	UFUNCTION(BlueprintPure, Category = "Extraction")
	bool IsExtractAvailable() const
	{
		return bExtractAvailable;
	}

	UPROPERTY(BlueprintAssignable, Category = "Extraction")
	FOnExtractRequested OnExtractRequested;

  protected:
	virtual void BeginPlay() override;

	virtual void PerformInteract(AActor *Interactor) override;
	virtual bool CanInteract(AActor *Interactor,
							 FText &OutReason) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Extraction")
	TObjectPtr<UStaticMeshComponent> MeshComp;

	// 테스트할때 true = 처음부터 열려있게
	UPROPERTY(EditAnywhere, Category = "Extraction")
	bool bStartAvailable = false;

	// 잠겨 있을 때 보여줄 문구는 PromptData.DisableReason 을 쓴다.
	// 생성자에서 기본값을 넣어두므로 에디터에서 바꾸면 된다.

  private:
	bool bExtractAvailable = false;
};
