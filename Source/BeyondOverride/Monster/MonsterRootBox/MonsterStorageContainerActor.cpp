// 26/09/28 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/MonsterRootBox/MonsterStorageContainerActor.h"

// Add include
#include "Components/SkeletalMeshComponent.h"
#include "DataTables/Monster/MonsterStorageInfo.h"

AMonsterStorageContainerActor::AMonsterStorageContainerActor()
{
	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMeshComp->SetEnableGravity(true);
	SkeletalComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Comp"));
	SkeletalComp->SetupAttachment(StaticMeshComp);
}

void AMonsterStorageContainerActor::MeshInfoSetUp(FName ID,
												  FVector Scale,
												  FVector Location,
												  FRotator Rotation,
												  USkeletalMesh* Skeletal,
												  TSubclassOf<UAnimInstance> Anim)
{
	StorageID = ID;
	SkeletalScale = Scale;
	SkeletalLocation = Location;
	SkeletalRotation = Rotation;
	StorageSkeletal = Skeletal;
	StorageAnimInstance = Anim;
	bInfoSetUpComplete = true;

	SkeletalComp->SetSkeletalMesh(StorageSkeletal);
	SkeletalComp->SetRelativeScale3D(SkeletalScale);
	SkeletalComp->SetRelativeLocation(SkeletalLocation);
	SkeletalComp->SetRelativeRotation(SkeletalRotation);

	if (StorageAnimInstance)
	{
		SkeletalComp->SetAnimInstanceClass(StorageAnimInstance);
	}

	PromptData.Title = FText::FromString(StorageID.ToString());
}

void AMonsterStorageContainerActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	GetWorld()->GetTimerManager().SetTimer(DeleteContainer,
										   this,
										   &AMonsterStorageContainerActor::EraseContainer,
										   180.00f,
										   false);
}

void AMonsterStorageContainerActor::EraseContainer()
{
	Destroy();
}
