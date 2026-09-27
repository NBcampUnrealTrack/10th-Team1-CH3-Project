
#pragma once

#include "CoreMinimal.h"

#include "ThrowableItemDataRow.generated.h"

class AThrowableBase;

USTRUCT(BlueprintType)
struct BEYONDOVERRIDE_API FThrowableItemDataRow : public FTableRowBase
{
	GENERATED_BODY()

  public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Throws")
	TSubclassOf<AThrowableBase> ThrowableClass; // 투척 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Throws")
	float ThrowDuration = 0.f; // 투척에 걸리는 시간
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Throws")
	float ThrowForce = 0.f; // 던지는 힘
};
