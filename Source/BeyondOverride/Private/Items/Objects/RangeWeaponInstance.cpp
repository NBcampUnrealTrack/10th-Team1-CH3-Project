#include "Items/Objects/RangeWeaponInstance.h"

#include "Items/DataAssets/RangeWeaponDataAsset.h"

URangeWeaponInstance::URangeWeaponInstance()
{
	RangeWeaponData = nullptr;

	CurrentAmmo = 0;
}

const URangeWeaponDataAsset* URangeWeaponInstance::GetRangeWeaponData() const
{
	return RangeWeaponData;
}

bool URangeWeaponInstance::ConsumeAmmo()
{
	if (CurrentAmmo > 0)
	{
		CurrentAmmo--;
		return true;
	}

	return false;
}

void URangeWeaponInstance::AddAmmo(int32& Amount)
{
	// 추가할 양이 부족한 경우
	if (Amount <= 0)
	{
		return;
	}

	// 탄창 가득찬 경우
	if (CurrentAmmo >= RangeWeaponData->MagazineSize)
	{
		return;
	}

	// 필요한 탄약 개수
	int32 RequiredAmmo = RangeWeaponData->MagazineSize - CurrentAmmo;

	// 주어진 탄약 전부 추가
	if (RequiredAmmo >= Amount)
	{
		CurrentAmmo += Amount;
		Amount = 0;
	}
	// 탄창 가득 추가
	else
	{
		CurrentAmmo = RangeWeaponData->MagazineSize;
		Amount -= RequiredAmmo;
	}
}

int32 URangeWeaponInstance::GetCurrentAmmo() const
{
	return CurrentAmmo;
}
