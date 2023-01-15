#include "ZombieSpawner.h"

#include "Kismet/GameplayStatics.h"
#include "Plagued_Arcade/Plagued_ArcadeGameMode.h"

AZombieSpawner::AZombieSpawner()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void AZombieSpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnTimer = SpawnInterval;
}

void AZombieSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Zombie_BP)
	{
		if (SpawnTimer > 0)
		{
			SpawnTimer -= DeltaTime;
		}
		else if (APlagued_ArcadeGameMode* gameMode = Cast<APlagued_ArcadeGameMode>(GetWorld()->GetAuthGameMode()))
		{
			if (gameMode->CurrentZombieCount > 0)
			{
				gameMode->CurrentZombieCount--;
				SpawnTimer = SpawnInterval;
				GetWorld()->SpawnActor<AZombie>(Zombie_BP, GetActorLocation(), GetActorRotation());
			}
		}
	}
}

