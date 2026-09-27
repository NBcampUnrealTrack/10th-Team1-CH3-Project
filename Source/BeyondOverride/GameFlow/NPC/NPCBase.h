// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"
#include "Interaction/Internal/InteractableInterface.h"

#include "NPCBase.generated.h"

class UInteractionComponent;
class UBehaviorComponent;
class UDialogueComponent;

UCLASS()
class BEYONDOVERRIDE_API ANPCBase : public ACharacter, public IInteractableInterface
{
	GENERATED_BODY()

  public:
	ANPCBase();

  protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	virtual FInteractPrompt GetInteractPrompt(AActor* Interactor) const override;
	virtual void OnInteractComplete(AActor* Interactor) override;
	virtual void PerformInteract(AActor* Interactor);

  public:
	FName GetNPCID() const;
	TObjectPtr<UInteractionComponent> GetInteractionComp() const;
	TObjectPtr<UBehaviorComponent> GetBehaviorComp() const;
	TObjectPtr<UDialogueComponent> GetDialogueComp() const;

  public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC")
	FName NPCID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC")
	TObjectPtr<UInteractionComponent> InteractionComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC")
	TObjectPtr<UBehaviorComponent> BehaviorComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC")
	TObjectPtr<UDialogueComponent> DialogueComp;

  protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	FInteractPrompt PromptData;
};
