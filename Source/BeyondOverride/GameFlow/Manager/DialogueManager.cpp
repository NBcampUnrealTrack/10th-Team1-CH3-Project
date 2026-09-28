// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/Manager/DialogueManager.h"

#include "DataAssets/BODataAsset.h"
#include "GameFlow/BOGameInstance.h"
#include "Logging/BOLog.h"

void UDialogueManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	TopicDatas.Empty();
	DialogueInteractionDatas.Empty();
	DialogueDatas.Empty();

	NPCTopicDatas.Empty();
	NPCDialogueInteractionDatas.Empty();

	LoadTopicData();
	LoadDialogueInteractionData();
	LoadDialogueData();
}

void UDialogueManager::LoadTopicData()
{
	if (!GetWorld())
	{
		return;
	}

	UBOGameInstance* GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>();
	if (!GameInstance)
	{
		return;
	}

	UBODataAsset* DataAsset = GameInstance->GetBODataAsset();
	if (!DataAsset)
	{
		return;
	}

	UDataTable* TopicDataTable = DataAsset->GetTopicDataTable();
	if (!TopicDataTable)
	{
		return;
	}

	const TMap<FName, uint8*>& AllRows = TopicDataTable->GetRowMap();

	for (const TPair<FName, uint8*>& Pair : AllRows)
	{
		if (FTopicData* Row = reinterpret_cast<FTopicData*>(Pair.Value))
		{
			FName TopicID = Pair.Key;
			FName NPCID = (*Row).NPCID;

			TopicDatas.Add(TopicID, *Row);

			NPCTopicDatas.FindOrAdd(NPCID).Add(TopicID);
		}
	}
}

void UDialogueManager::LoadDialogueInteractionData()
{
	if (!GetWorld())
	{
		return;
	}

	UBOGameInstance* GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>();
	if (!GameInstance)
	{
		return;
	}

	UBODataAsset* DataAsset = GameInstance->GetBODataAsset();
	if (!DataAsset)
	{
		return;
	}

	UDataTable* DialogueInteractionDataTable = DataAsset->GetDialogueInteractionDataTable();
	if (!DialogueInteractionDataTable)
	{
		return;
	}

	const TMap<FName, uint8*>& AllRows = DialogueInteractionDataTable->GetRowMap();

	for (const TPair<FName, uint8*>& Pair : AllRows)
	{
		if (FDialogueInteractionData* Row = reinterpret_cast<FDialogueInteractionData*>(Pair.Value))
		{
			FName DialogueID = Pair.Key;
			FName NPCID = (*Row).NPCID;
			EDialogueSituation DialogueSituation = (*Row).DialogueSituation;

			DialogueInteractionDatas.Emplace(DialogueID, *Row);

			NPCDialogueInteractionDatas.FindOrAdd(NPCID).Emplace(DialogueSituation, DialogueID);
		}
	}
}

void UDialogueManager::LoadDialogueData()
{
	if (!GetWorld())
	{
		return;
	}

	UBOGameInstance* GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>();
	if (!GameInstance)
	{
		return;
	}

	UBODataAsset* DataAsset = GameInstance->GetBODataAsset();
	if (!DataAsset)
	{
		return;
	}

	UDataTable* DialogueDataTable = DataAsset->GetDialogueDataTable();
	if (!DialogueDataTable)
	{
		return;
	}

	const TMap<FName, uint8*>& AllRows = DialogueDataTable->GetRowMap();

	for (const TPair<FName, uint8*>& Pair : AllRows)
	{
		if (FDialogueData* Row = reinterpret_cast<FDialogueData*>(Pair.Value))
		{
			FName DialogueID = Pair.Key;

			DialogueDatas.Emplace(DialogueID, *Row);
		}
	}
}

void UDialogueManager::GetTopicData(FName TopicID, FTopicData& Data) const
{
	if (TopicDatas.Contains(TopicID))
	{
		Data = TopicDatas[TopicID];
	}
}

void UDialogueManager::GetDialogueInteractionData(FName DialogueInteractionID, FDialogueInteractionData& Data) const
{
	if (DialogueInteractionDatas.Contains(DialogueInteractionID))
	{
		Data = DialogueInteractionDatas[DialogueInteractionID];
	}
	else
	{
		UE_LOG(LogGameFlow, Warning, TEXT("No DialogueInteractionDatas"));
	}
}

void UDialogueManager::GetDialogueData(FName DialogueID, FDialogueData& Data) const
{
	if (DialogueDatas.Contains(DialogueID))
	{
		Data = DialogueDatas[DialogueID];
	}
}

void UDialogueManager::GetNPCTopicDatas(FName NPCID, TArray<FName>& Data) const
{
	if (NPCTopicDatas.Contains(NPCID))
	{
		Data = NPCTopicDatas[NPCID];
	}
}

void UDialogueManager::GetNPCDialogueInteractionDatas(FName NPCID, TMap<EDialogueSituation, FName>& Data) const
{
	if (NPCDialogueInteractionDatas.Contains(NPCID))
	{
		Data = NPCDialogueInteractionDatas[NPCID];
	}
}

FName UDialogueManager::GetNPCDialogueInteractionData(FName NPCID, EDialogueSituation DialogueSituation) const
{
	FName Result = FName(TEXT("Default"));

	if (NPCDialogueInteractionDatas.Contains(NPCID) && NPCDialogueInteractionDatas[NPCID].Contains(DialogueSituation))
	{
		return NPCDialogueInteractionDatas[NPCID][DialogueSituation];
	}

	UE_LOG(LogGameFlow, Warning, TEXT("No NPCDIData"));
	return Result;
}
