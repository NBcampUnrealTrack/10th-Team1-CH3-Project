// 26/09/26 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/ActorComponent/BossPhaseComponent.h"

UBossPhaseComponent::UBossPhaseComponent()
{
	Phase = EBossPhase::Phase1;
}

// Public Functions
void UBossPhaseComponent::SetPhase1()
{
	Phase = EBossPhase::Phase1;
}

void UBossPhaseComponent::SetPhase2()
{
	Phase = EBossPhase::Phase2;
}

EBossPhase UBossPhaseComponent::CurrentPhase() const
{
	return Phase;
}
