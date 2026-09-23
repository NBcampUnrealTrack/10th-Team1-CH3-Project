// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/Manager/RegionManager.h"

#include "ContainerManager.h"
#include "ExitManager.h"
#include "SpawnVolumeManager.h"

#include "GameFlow/BOGameInstance.h"

void URegionManager::InitSetting()
{
	if (!GetWorld() || !GetWorld()->GetGameInstance())
	{
		return;
	}

	if (USpawnVolumeManager* SpawnVolumeManger = GetWorld()->GetGameInstance()->GetSubsystem<USpawnVolumeManager>())
	{
		SpawnVolumeManger->InitSetting();
	}

	if (UContainerManager* ContainerManager = GetWorld()->GetGameInstance()->GetSubsystem<UContainerManager>())
	{
		ContainerManager->InitSetting();
	}

	if (UExitManager* ExitManager = GetWorld()->GetGameInstance()->GetSubsystem<UExitManager>())
	{
		ExitManager->InitSetting();
	}
}

void URegionManager::CleanSetting()
{
	if (!GetWorld() || !GetWorld()->GetGameInstance())
	{
		return;
	}

	UBOGameInstance* GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>();
	if (!GameInstance)
	{
		return;
	}

	if (USpawnVolumeManager* SpawnVolumeManger = GetWorld()->GetGameInstance()->GetSubsystem<USpawnVolumeManager>())
	{
		SpawnVolumeManger->CleanSetting();
	}

	if (UContainerManager* ContainerManager = GetWorld()->GetGameInstance()->GetSubsystem<UContainerManager>())
	{
		ContainerManager->CleanSetting();
	}

	if (UExitManager* ExitManager = GetWorld()->GetGameInstance()->GetSubsystem<UExitManager>())
	{
		ExitManager->CleanSetting();
	}
}
