#include "Items/Objects/RangeWeaponInstance.h"

URangeWeaponInstance::URangeWeaponInstance()
{
	RangeWeaponData = nullptr;

	CurrentAmmo = 0;
}

const URangeWeaponDataAsset* URangeWeaponInstance::GetRangeWeaponData() const
{
	return RangeWeaponData;
}

int32 URangeWeaponInstance::GetCurrentAmmo() const
{
	return CurrentAmmo;
}

bool URangeWeaponInstance::ConsumeAmmo()
{
	return false;
}

int32 URangeWeaponInstance::AddAmmo(int32 Amount)
{
	return int32();
}
