#include "Items/Objects/RangeWeaponInstance.h"

#include "DataTables/Items/RangeWeaponDataRow.h"
#include "Subsystems/ItemDataSubsystem.h"

URangeWeaponInstance::URangeWeaponInstance()
{
	RangeWeaponData = nullptr;

	CurrentAmmo = 0;
}

void URangeWeaponInstance::Initialize()
{
	Super::Initialize();

	UE_LOG(LogTemp, Warning, TEXT("[URangeWeaponInstance] Initialize %s"), *GetNameSafe(this));

	// RangeWeaponData 로드
	UItemDataSubsystem* ItemDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UItemDataSubsystem>();
	RangeWeaponData = ItemDataSubsystem->GetRangeWeaponData(ItemID);
}

const FRangeWeaponDataRow* URangeWeaponInstance::GetRangeWeaponData() const
{
	return RangeWeaponData;
}

bool URangeWeaponInstance::ConsumeAmmo()
{
	// 탄약이 존재하는 경우 - 탄약 소모 성공
	if (CurrentAmmo > 0)
	{
		CurrentAmmo--;
		return true;
	}

	// 탄약 소모 실패
	return false;
}

int32 URangeWeaponInstance::AddAmmo(int32 Amount)
{
	// 추가할 양이 부족한 경우
	if (Amount <= 0)
	{
		return 0;
	}

	// 탄창 가득찬 경우
	if (CurrentAmmo >= RangeWeaponData->MagazineSize)
	{
		return 0;
	}

	// 필요한 탄약 개수
	int32 RequiredAmmo = RangeWeaponData->MagazineSize - CurrentAmmo;

	// 주어진 탄약이 필요한 개수 이하인 경우
	if (RequiredAmmo >= Amount)
	{
		CurrentAmmo += Amount;
		return 0;
	}

	// 추가 후 남는 탄약 개수 반환
	CurrentAmmo = RangeWeaponData->MagazineSize;
	return Amount - RequiredAmmo;
}

int32 URangeWeaponInstance::GetCurrentAmmo() const
{
	return CurrentAmmo;
}

int32 URangeWeaponInstance::GetMagazineSize() const
{
	if (!RangeWeaponData)
	{
		return 0;
	}

	return RangeWeaponData->MagazineSize;
}
