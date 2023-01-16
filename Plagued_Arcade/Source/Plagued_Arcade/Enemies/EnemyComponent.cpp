#include "EnemyComponent.h"

#include "Plagued_Arcade/PlayerStates/PlaguedPlayerState.h"

UEnemyComponent::UEnemyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	CurrentHealth = MaxHealth;
}

void UEnemyComponent::TakeDamage(int _amount)
{
	CurrentHealth -= _amount;
	if (CurrentHealth <= 0)
	{
		CurrentHealth = 0;
		
		if (APlaguedPlayerState* localPlayerState = GetWorld()->GetFirstPlayerController()->GetPlayerState<APlaguedPlayerState>())
		{
			localPlayerState->CurrentPoints += WorthInPoints;
		}
	}
	else
	{
		if (APlaguedPlayerState* localPlayerState = GetWorld()->GetFirstPlayerController()->GetPlayerState<APlaguedPlayerState>())
		{
			localPlayerState->CurrentPoints += 10;
		}
	}
}

