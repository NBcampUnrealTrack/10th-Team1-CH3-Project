#include "Throwables/ImpactThrowable.h"

AImpactThrowable::AImpactThrowable()
{
	// Hit 이벤트 바인딩
	SkeletalMesh->SetNotifyRigidBodyCollision(true);
	SkeletalMesh->SetAllBodiesNotifyRigidBodyCollision(true);
	SkeletalMesh->OnComponentHit.AddDynamic(this, &AImpactThrowable::OnHit);
}

void AImpactThrowable::OnHit(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	UE_LOG(
		LogTemp,
		Warning,
		TEXT("HIT: OtherActor=%s, OtherComp=%s, Normal=%s"),
		OtherActor ? *OtherActor->GetName() : TEXT("None"),
		OtherComp ? *OtherComp->GetName() : TEXT("None"),
		*Hit.Normal.ToString());

	Activate();
}
