#include "Interaction/Internal/InteractHighlightComponent.h"

#include "Components/MeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"

UInteractHighlightComponent::UInteractHighlightComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// 기본 머티리얼을 코드에서 찾아둠
	// 없으면 강조만 안 켜진다.
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> DefaultMat(
		TEXT("/Game/Interaction/Materials/M_InteractHighlight.M_InteractHighlight"));

	if (DefaultMat.Succeeded())
	{
		HighlightMaterial = DefaultMat.Object;
	}
}

void UInteractHighlightComponent::SetHighlighted(bool bOn)
{
	if (bHighlighted == bOn)
	{
		return;
	}

	AActor *Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	if (bOn && !HighlightMaterial)
	{
		UE_LOG(LogTemp, Warning,
			   TEXT("%s: HighlightMaterial 이 비어 있어 강조를 켤 수 없습니다."),
			   *Owner->GetName());
		return;
	}

	bHighlighted = bOn;

	// 액터 안의 모든 메시에 적용한다.
	// UMeshComponent 로 받으므로 스태틱이든 스켈레탈이든 상관없다
	// NPC(스켈레탈)와 창고(스태틱)가 같은 코드로 처리된다.
	TArray<UMeshComponent *> Meshes;
	Owner->GetComponents<UMeshComponent>(Meshes);

	for (UMeshComponent *Mesh : Meshes)
	{
		if (Mesh)
		{
			Mesh->SetOverlayMaterial(bOn ? HighlightMaterial : nullptr);
		}
	}
}
