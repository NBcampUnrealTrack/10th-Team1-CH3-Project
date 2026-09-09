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

bool URangeWeaponInstance::ConsumeAmmo()
{
	return false;
}

void URangeWeaponInstance::AddAmmo(int32& Amount)
{
}

int32 URangeWeaponInstance::GetCurrentAmmo() const
{
	return CurrentAmmo;
}
