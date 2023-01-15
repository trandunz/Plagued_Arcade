#include "EnemyComponent.h"

UEnemyComponent::UEnemyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	CurrentHealth = MaxHealth;
}

void UEnemyComponent::TakeDamage(int _amount)
{
	CurrentHealth -= _amount;
	if (CurrentHealth < 0)
		CurrentHealth = 0;
}

