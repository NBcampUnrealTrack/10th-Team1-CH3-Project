#pragma once

#include "CoreMinimal.h"

#include "Interaction/InteractableActorBase.h"

#include "ExitActor.generated.h"

UCLASS()
class BEYONDOVERRIDE_API AExitActor : public AInteractableActorBase
{
	GENERATED_BODY()

  public:
	AExitActor();

	// 탈출구를 열거나 닫는다.
	UFUNCTION(BlueprintCallable, Category = "Extraction")
	void SetExtractAvailable(bool bAvailable);

	UFUNCTION(BlueprintPure, Category = "Extraction")
	bool IsExtractAvailable() const
	{
		return PromptData.bEnabled;
	}

  protected:
	virtual void PerformInteract(AActor* Interactor) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Extraction")
	TObjectPtr<UStaticMeshComponent> MeshComp;
};
