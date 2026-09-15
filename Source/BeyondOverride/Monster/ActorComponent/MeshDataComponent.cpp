// 26/09/14 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/ActorComponent/MeshDataComponent.h"

UMeshDataComponent::UMeshDataComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

UParticleSystem* UMeshDataComponent::GetEffect() const
{
	return AttackEffect;
}

void UMeshDataComponent::SetEffect(UParticleSystem* Effect)
{
	AttackEffect = Effect;
}

FName UMeshDataComponent::GetAttackSocket() const
{
	return AttackSocket;
}

void UMeshDataComponent::SetAttackSocket(FName Socket)
{
	AttackSocket = Socket;
}

USkeletalMesh* UMeshDataComponent::GetSkeletal() const
{
	return TargetSkeletal;
}

void UMeshDataComponent::SetSkeletal(USkeletalMesh* Skeletal)
{
	TargetSkeletal = Skeletal;
}

UClass* UMeshDataComponent::GetAnim() const
{
	return TargetAnimInstance;
}

void UMeshDataComponent::SetAnim(UClass* Animation)
{
	TargetAnimInstance = Animation;
}

FName UMeshDataComponent::GetDefaultSkeletal() const
{
	return DefaultTargetSkeletal;
}

FName UMeshDataComponent::GetDefaultAnim() const
{
	return DefaultTargetAnimInstance;
}

void UMeshDataComponent::BeginPlay()
{
	Super::BeginPlay();
}
