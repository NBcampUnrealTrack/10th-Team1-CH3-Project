// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/Manager/ContainerManager.h"

#include "../BOGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UContainerManager::Initialize()
{
	if (!GetWorld())
	{
		return;
	}

	TArray<AActor*> AllActors{};  // AContainer*로 변경
	// UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);  // AContainer로 변경
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Container"), AllActors);  // test code

	for (AActor* Actor : AllActors)
	{
		// Container로 캐스팅

		Containers.Add(Actor);  // Container 넣기
	}
}

void UContainerManager::ActivateContainer()
{
	UBOGameInstance* GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>();
	if (!GameInstance)
	{
		return;
	}

	for (TObjectPtr<AActor> Container : Containers)  // AActor -> AContainer로 변경
	{
		float Probability = FMath::RandRange(0.0f, 100.0f);

		if (Probability >= ActivateProbability)
		{
			// Container Data 채우기
			// Container의 TArray<FName> Items 멤버 설정
			// 이후 상호작용 시 인벤토리 컴포넌트에서 해당 데이터 가지고 인벤토리 구성
		}
	}
}
