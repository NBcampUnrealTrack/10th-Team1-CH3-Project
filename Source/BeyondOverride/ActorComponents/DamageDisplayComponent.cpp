#include "ActorComponents/DamageDisplayComponent.h"

#include "Components/WidgetComponent.h"

UDamageDisplayComponent::UDamageDisplayComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	DamageDisplayWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Damage Display Widget"));
	DamageDisplayWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

void UDamageDisplayComponent::BeginPlay()
{
	Super::BeginPlay();

	DamageDisplayWidget->SetupAttachment(GetOwner()->GetRootComponent());
}
