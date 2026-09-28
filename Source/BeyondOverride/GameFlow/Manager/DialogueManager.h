// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DataTables/NPC/NPCDialogueData.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "DialogueManager.generated.h"

/**
 *
 */
UCLASS()
class BEYONDOVERRIDE_API UDialogueManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

  public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

  private:
	void LoadTopicData();
	void LoadDialogueInteractionData();
	void LoadDialogueData();

  public:
	void GetTopicData(FName TopicID, FTopicData& Data) const;
	void GetDialogueInteractionData(FName DialogueInteractionID, FDialogueInteractionData& Data) const;
	void GetDialogueData(FName DialogueID, FDialogueData& Data) const;

	void GetNPCTopicDatas(FName NPCID, TArray<FName>& Data) const;
	void GetNPCDialogueInteractionDatas(FName NPCID, TMap<EDialogueSituation, FName>& Data) const;
	FName GetNPCDialogueInteractionData(FName NPCID, EDialogueSituation DialogueSituation) const;

  private:
	TMap<FName, FTopicData> TopicDatas;
	TMap<FName, FDialogueInteractionData> DialogueInteractionDatas;
	TMap<FName, FDialogueData> DialogueDatas;

	TMap<FName, TArray<FName>> NPCTopicDatas;                                 // <NPCID, TopicID>
	TMap<FName, TMap<EDialogueSituation, FName>> NPCDialogueInteractionDatas; // <NPCID, <EDialogueSituation, DialogueInteractionID>>
};
