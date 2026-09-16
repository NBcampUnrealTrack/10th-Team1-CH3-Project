// 26/09/14 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/ActorComponent/ShortTermStateComponent.h"

UShortTermStateComponent::UShortTermStateComponent()
{
}

void UShortTermStateComponent::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(FlagControlTimer,
										   this,
										   &UShortTermStateComponent::PopFlag,
										   2.0f,
										   true);
}

void UShortTermStateComponent::PlantFlag(FFlagInfo FlagInfo)
{
	Flags.Add(FlagInfo);
}

void UShortTermStateComponent::PlantFlag(EFlag State, float Time)
{
	FFlagInfo Item;
	Item.Flag = State;
	Item.CallTime = Time;

	Flags.Add(Item);
}

void UShortTermStateComponent::PlantFlag(EFlag State, float Time, bool Type)
{
	FFlagInfo Item;
	Item.Flag = State;
	Item.CallTime = Time;
	Item.FlagType = Type;

	Flags.Add(Item);
}

bool UShortTermStateComponent::FoldFlags(EFlag Target)
{
	bool IsFold = false;

	const float CurrentTime = GetWorld()->GetTimeSeconds();

	for (size_t index = 0; index < Flags.Num(); index = index + 1)
	{
		if (Flags[index].Flag == Target && CurrentTime - Flags[index].CallTime <= 0.1f)
		{
			Flags[index].Complete = true;
			IsFold = true;
		}
	}

	return IsFold;
}

bool UShortTermStateComponent::FoldFlags(EFlag Target, bool& Type)
{
	bool IsFold = false;

	const float CurrentTime = GetWorld()->GetTimeSeconds();

	for (size_t index = 0; index < Flags.Num(); index = index + 1)
	{
		if (Flags[index].Flag == Target && CurrentTime - Flags[index].CallTime <= 0.1f)
		{
			Flags[index].Complete = true;
			IsFold = true;
			if (Flags[index].FlagType)
			{
				Type = true;
			}
		}
	}

	return IsFold;
}

void UShortTermStateComponent::PopFlag()
{
	const float CurrentTime = GetWorld()->GetTimeSeconds();

	Flags.RemoveAll([CurrentTime](const FFlagInfo& Flag)
					{ return CurrentTime - Flag.CallTime > 0.1f; });

	Flags.RemoveAll([CurrentTime](const FFlagInfo& Flag)
					{ return Flag.Complete == true; });
}
