#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Plagued_Arcade/Enemies/Zombie.h"
#include "ZombieSpawner.generated.h"

UCLASS()
class PLAGUED_ARCADE_API AZombieSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AZombieSpawner();
	
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemyType, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AZombie> Zombie_BP;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpawnSettings, meta = (AllowPrivateAccess = "true"))
	float SpawnInterval = 2.0f;
	float SpawnTimer = 0.0f;
};
