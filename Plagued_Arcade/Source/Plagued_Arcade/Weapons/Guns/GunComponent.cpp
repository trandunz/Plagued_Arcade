#include "GunComponent.h"

UGunComponent::UGunComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGunComponent::Reload()
{
	CurrentAmmo = MaxAmmo;
}

void UGunComponent::Fire()
{
	if (CurrentAmmo > 0)
		CurrentAmmo--;
}
