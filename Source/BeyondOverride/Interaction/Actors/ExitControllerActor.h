#pragma once

#include "CoreMinimal.h"

#include "Interaction/InteractableActorBase.h"

#include "ExitControllerActor.generated.h"

class AExitActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnExtractControlRequested,
											 AExitControllerActor*, ExitController,
											 AActor*, Interactor);

UCLASS()
class BEYONDOVERRIDE_API AExitControllerActor : public AInteractableActorBase
{
	GENERATED_BODY()

  public:
	AExitControllerActor();

	UFUNCTION(BlueprintCallable, Category = "Extraction")
	void SetControllerAvailable(bool bNewEnabled, const FText& Reason = FText::GetEmpty());

	UFUNCTION(BlueprintCallable, Category = "Extraction")
	AExitActor* GetTargetExit() const
	{
		return TargetExit;
	};

	UFUNCTION(BlueprintCallable, Category = "Extraction")
	FName GetRegionId() const
	{
		return RegionId;
	};

  protected:
	virtual void BeginPlay() override;

	virtual void PerformInteract(AActor* Interactor) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UStaticMeshComponent> MeshComp;

  public:
	UPROPERTY(EditInstanceOnly, Category = "Extraction")
	TObjectPtr<AExitActor> TargetExit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Extraction")
	FName RegionId = "Default";

	UPROPERTY(BlueprintAssignable, Category = "Extraction")
	FOnExtractControlRequested OnExtractControlRequested;
	float ControlTime = 3.f;

  private:
	void SetExitActorOpenTimer();

	FTimerHandle ControlTimer;
};
