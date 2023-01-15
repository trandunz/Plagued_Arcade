#include "ZombieSpawner.h"

AZombieSpawner::AZombieSpawner()
{
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
		else
		{
			SpawnTimer = SpawnInterval;
			AActor* newZombie = GetWorld()->SpawnActor(Zombie_BP);
			newZombie->SetActorLocation(GetActorLocation());
		}
	}
}

