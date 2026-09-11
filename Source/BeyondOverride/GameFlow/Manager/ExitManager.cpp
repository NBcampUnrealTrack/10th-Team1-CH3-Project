// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/Manager/ExitManager.h"

#include "Kismet/GameplayStatics.h"

void UExitManager::Initialize()
{
	if (!GetWorld())
	{
		return;
	}

	TArray<AActor*> AllActors{};  // AExit*로 변경
	// UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);  // AExit로 변경
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Exit"), AllActors);  // test code

	for (AActor* Actor : AllActors)
	{
		// Exit로 캐스팅

		Exits.Add(Actor);  // Container 넣기
	}
}

void UExitManager::ActivateExit()
{
}

AActor* UExitManager::SelectRandomExit()
{
	return nullptr;
}
