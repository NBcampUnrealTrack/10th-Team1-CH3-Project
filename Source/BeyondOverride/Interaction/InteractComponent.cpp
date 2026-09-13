#include "Interaction/InteractComponent.h"

#include "DrawDebugHelpers.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "Interaction/Internal/InteractableInterface.h"
#include "Interaction/Internal/InteractionChannels.h"

UInteractComponent::UInteractComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (!Owner)
		return;

	UCameraComponent* Camera = Owner->FindComponentByClass<UCameraComponent>();
	USceneComponent* AttachTarget = Camera ? static_cast<USceneComponent*>(Camera) : Owner->GetRootComponent();
	if (!AttachTarget)
		return;

	const float CameraOffset = FVector::Dist(AttachTarget->GetComponentLocation(), Owner->GetActorLocation());
	const float TotalLength = TraceDistance + CameraOffset;
	const float HalfHeight = FMath::Max(TotalLength * 0.5f, TraceRadius);

	DetectionCollision = NewObject<UCapsuleComponent>(Owner, TEXT("InteractDetactionCollision"));
	if (DetectionCollision)
	{
		DetectionCollision->SetCapsuleSize(TraceRadius, HalfHeight);
		DetectionCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		DetectionCollision->SetGenerateOverlapEvents(true);
		//DetectionCollision->SetCollisionObjectType(ECC_InteractionDetector);
		DetectionCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
		//DetectionCollision->SetCollisionResponseToChannel(ECC_InteractionDetector, ECR_Overlap);

		DetectionCollision->SetupAttachment(AttachTarget);
		DetectionCollision->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
		DetectionCollision->SetRelativeLocation(FVector(HalfHeight, 0.f, 0.f));

		DetectionCollision->RegisterComponent();

		//DetectionCollision->OnComponentBeginOverlap.AddDynamic(this, &UInteractComponent::OnDetectionBeginOverlap);
		//DetectionCollision->OnComponentEndOverlap.AddDynamic(this, &UInteractComponent::OnDetectionEndOverlap);
	}
}

void UInteractComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (DetectionCollision)
	{
		DetectionCollision->DestroyComponent();
		DetectionCollision = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

void UInteractComponent::TickComponent(
    float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bDrawDebug && DetectionCollision && GetWorld())
	{
		DrawDebugCapsule(
			GetWorld(),
			DetectionCollision->GetComponentLocation(),
			DetectionCollision->GetScaledCapsuleHalfHeight(),
			DetectionCollision->GetScaledCapsuleRadius(),
			DetectionCollision->GetComponentQuat(),
			FColor::Cyan,
			false, -1.f, 0, 1.f);
	}

	FVector ViewLoc, TraceEnd, HitPoint;
	bool bHitSomething = false;

    AActor *Target = TraceForTarget(ViewLoc, TraceEnd, bHitSomething, HitPoint);

    SetFocus(Target);

    if (bDrawDebug && GetWorld())
    {
        // 초록 = 상호작용 가능, 노랑 = 뭔가 맞았지만 상호작용 불가,
        // 빨강 = 아무것도 안 맞음
        FColor LineColor = FColor::Red;
        if (bHitSomething)
            LineColor = FColor::Yellow;
        if (Target)
            LineColor = FColor::Green;

		DrawDebugLine(GetWorld(), ViewLoc, TraceEnd, LineColor, false, -1.f, 0, 1.f);

		if (bHitSomething)
		{
			DrawDebugPoint(GetWorld(), HitPoint, 12.f, FColor::White, false, -1.f);
		}
	}

    PushPrompt();
    UpdateHold(DeltaTime);
}

// 광선 쏘기
AActor* UInteractComponent::TraceForTarget(FVector& OutViewLoc,
										   FVector& OutTraceEnd,
										   bool& bOutHitSomething,
										   FVector& OutHitPoint) const
{
    OutViewLoc = FVector::ZeroVector;
    OutTraceEnd = FVector::ZeroVector;
    bOutHitSomething = false;
    OutHitPoint = FVector::ZeroVector;

    AActor *Owner = GetOwner();
    UWorld *World = GetWorld();
    if (!Owner || !World)
    {
        return nullptr;
    }

    // GetPlayerViewPoint 는 "지금 화면이 보고 있는 곳"을 돌려준다.
    // 1인칭이든 3인칭이든 알아서 맞는 값이 나오므로,
    // 시점이 바뀌어도 이 코드는 안 고쳐도 된다.
    APawn *Pawn = Cast<APawn>(Owner);
    AController *Ctrl = Pawn ? Pawn->GetController() : nullptr;
    if (!Ctrl)
    {
        // 조종하는 컨트롤러가 없다 = 언포제스,사망, 컷신
        return nullptr;
    }

    FRotator ViewRot;
    Ctrl->GetPlayerViewPoint(OutViewLoc, ViewRot);

	// 3인칭 보정
	// 카메라가 캐릭터 뒤 500 쯤에 있으면 광선이 카메라에서 출발하므로,
	// TraceDistance 만큼만 쏘면 광선이 캐릭터 등 뒤에서 끝나 버린다.
	// 카메라~캐릭터 거리를 더해서 "캐릭터 기준 250" 이 되게 맞춘다.
	const float CameraOffset = FVector::Dist(OutViewLoc, Owner->GetActorLocation());

	OutTraceEnd = OutViewLoc + ViewRot.Vector() * (TraceDistance + CameraOffset);

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Owner);

	FCollisionShape CapsuleShape = FCollisionShape::MakeCapsule(TraceRadius, TraceRadius);

	FHitResult Hit;
	bOutHitSomething = World->SweepSingleByChannel(Hit, OutViewLoc, OutTraceEnd, FQuat::Identity, ECC_Interaction, CapsuleShape, Params);

	if (!bOutHitSomething)
		return nullptr;

    OutHitPoint = Hit.ImpactPoint;

	// 보정을 더했으니 실제 손이 닿는 거리인지 다시 확인한다.
	// 안 하면 멀리 있는 물건도 잡히게 된다.
	const float Reach =
		FVector::Dist(Owner->GetActorLocation(), Hit.ImpactPoint);
	if (Reach > TraceDistance)
		return nullptr;

    // 약속을 지킨 물건인지. 안 지켰으면 nullptr 이 나온다.
    if (!Cast<IInteractableInterface>(Hit.GetActor()))
    {
        return nullptr;
    }

    return Hit.GetActor();
}

// 대상 전환
void UInteractComponent::SetFocus(AActor *NewTarget)
{
    AActor *Old = FocusedActor.Get();
    if (Old == NewTarget)
    {
        return;
    }

    // 대상을 바꾸기 전에 홀드를 취소한다.
    // 순서가 중요하다. 대상을 먼저 바꾸면 취소 통보가 엉뚱한 액터에게 간다.
    if (bHolding)
    {
        CancelHold();
    }

    if (IInteractableInterface *OldI = Cast<IInteractableInterface>(Old))
    {
        OldI->OnFocusEnd(GetOwner());
    }

    FocusedActor = NewTarget;

    if (IInteractableInterface *NewI = Cast<IInteractableInterface>(NewTarget))
    {
        NewI->OnFocusBegin(GetOwner());
    }
}

// UI 통보
void UInteractComponent::PushPrompt()
{
    const IInteractableInterface *I =
        Cast<IInteractableInterface>(FocusedActor.Get());

    if (!I)
    {
        ClearPrompt();
        return;
    }

    const FInteractPrompt Prompt = I->GetInteractPrompt(GetOwner());

    // !bPromptVisible > 방금 대상이 생긴 순간. 값이 같아도 무조건 보낸다.
    //                   (UI 가 지금 숨겨져 있으므로 켜줘야 한다)
    // Prompt != Last  > 표시 내용이 실제로 바뀐 순간.
    //                   (맨홀이 "잠김" > "탈출 가능")
    //
    // 둘 다 아니면 아무것도 하지 않는다. 이게 절약되는 부분이다
    // 매 프레임 SetText 를 부르면 Slate 가 매번 다시 그린다.
    if (!bPromptVisible || Prompt != LastSentPrompt)
    {
        OnPromptChanged.Broadcast(true, Prompt);
        LastSentPrompt = Prompt;
        bPromptVisible = true;
    }
}

void UInteractComponent::ClearPrompt()
{
    if (!bPromptVisible)
    {
        return;
    }

    OnPromptChanged.Broadcast(false, FInteractPrompt());

    // 반드시 초기화한다.
    // 안 하면 같은 물건을 다시 쳐다봤을 때 "값이 안 바뀌었네" 하고
    // 안 보내서 프롬프트가 영영 안 뜬다.
    LastSentPrompt = FInteractPrompt();
    bPromptVisible = false;
}

// 켜고 끄기
void UInteractComponent::SetInteractionEnabled(bool bEnabled)
{
    if (bInteractionEnabled == bEnabled)
    {
        return;
    }

    bInteractionEnabled = bEnabled;

    if (!bEnabled)
    {
        // 끌 때는 확실히 정리한다.
        // 이 순서를 지켜야 UI 에 프롬프트가 남지 않는다.
        if (bHolding)
        {
            CancelHold();
        }
        SetFocus(nullptr);
        ClearPrompt();
    }

    // 광선도 안 쏘게 Tick 자체를 끈다.
    SetComponentTickEnabled(bEnabled);
}

// 입력
void UInteractComponent::PressInteract()
{
    if (!bInteractionEnabled)
    {
        return;
    }

    IInteractableInterface *I =
        Cast<IInteractableInterface>(FocusedActor.Get());
    if (!I)
    {
        return;
    }

    const FInteractPrompt Prompt = I->GetInteractPrompt(GetOwner());
    if (!Prompt.bEnabled)
    {
        // 실패 이유는 UI 가 이미 DisableReason 으로 받아 표시하고 있다.
        return;
    }

    I->OnInteractStart(GetOwner());

    // 홀드가 없으면 즉시 완료
    if (Prompt.HoldSeconds <= 0.f)
    {
        I->OnInteractComplete(GetOwner());
        return;
    }

    // 홀드 시작. 이 시점의 값을 복사해 둔다
    // 도중에 프롬프트가 바뀌어도 게이지가 튀지 않게
    bHolding = true;
    HoldElapsed = 0.f;
    HoldDuration = Prompt.HoldSeconds;
    bHoldMoveCancel = Prompt.bMoveCancel;
    HoldStartLocation =
        GetOwner() ? GetOwner()->GetActorLocation() : FVector::ZeroVector;
}

void UInteractComponent::ReleaseInteract()
{
    if (bHolding)
    {
        CancelHold();
    }
}

// 홀드
float UInteractComponent::GetHoldProgress() const
{
    if (!bHolding || HoldDuration <= 0.f)
    {
        return 0.f;
    }

    return FMath::Clamp(HoldElapsed / HoldDuration, 0.f, 1.f);
}

void UInteractComponent::UpdateHold(float DeltaTime)
{
    if (!bHolding)
    {
        return;
    }

    // 홀드 중에 대상이 사라졌거나 시선을 돌렸으면 취소
    IInteractableInterface *I =
        Cast<IInteractableInterface>(FocusedActor.Get());
    if (!I)
    {
        CancelHold();
        return;
    }

    if (bHoldMoveCancel && GetOwner())
    {
        const float Moved =
            FVector::Dist(HoldStartLocation, GetOwner()->GetActorLocation());
        if (Moved > MoveCancelDistance)
        {
            CancelHold();
            return;
        }
    }

    HoldElapsed += DeltaTime;

    OnHoldProgress.Broadcast(GetHoldProgress());

    if (HoldElapsed >= HoldDuration)
    {
        CompleteHold();
    }
}

void UInteractComponent::CancelHold()
{
    if (!bHolding)
    {
        return;
    }

    // 플래그를 먼저 내린다.
    // 그래야 아래 통보를 받은 쪽에서 GetHoldProgress() 를 물어도 0 이 나온다.
    bHolding = false;
    HoldElapsed = 0.f;

    OnHoldProgress.Broadcast(0.f);

	if (IInteractableInterface* I =
			Cast<IInteractableInterface>(FocusedActor.Get()))
	{
		I->OnInteractCancel(GetOwner());
	}
}

void UInteractComponent::CompleteHold()
{
    if (!bHolding)
    {
        return;
    }

    bHolding = false;
    HoldElapsed = 0.f;

    OnHoldProgress.Broadcast(0.f);

	if (IInteractableInterface* I =
			Cast<IInteractableInterface>(FocusedActor.Get()))
	{
		// 이 호출로 액터가 Destroy 될 수 있다 (아이템 줍기 등)
		// 이 줄 아래에서 I 나 FocusedActor 를 다시 쓰면 안 된다.
		I->OnInteractComplete(GetOwner());
	}
}

//void UInteractComponent::OnDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	UE_LOG(LogTemp, Warning, TEXT("BeginOverlap: %s"), OtherActor ? *OtherActor->GetName() : TEXT("NULL"));
//
//	if (!Cast<IInteractableInterface>(OtherActor))
//		return;
//
//	NearbyInteractableCount++;
//
//	if (NearbyInteractableCount >= 1)
//		SetComponentTickEnabled(true);
//}
//
//void UInteractComponent::OnDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	UE_LOG(LogTemp, Warning, TEXT("EndOverlap: %s"), OtherActor ? *OtherActor->GetName() : TEXT("NULL"));
//
//	if (!Cast<IInteractableInterface>(OtherActor))
//		return;
//
//	NearbyInteractableCount = FMath::Max(0, NearbyInteractableCount - 1);
//
//	if (NearbyInteractableCount <= 0)
//	{
//		SetComponentTickEnabled(false);
//
//		SetFocus(nullptr);
//		ClearPrompt();
//	}
//}
