#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Zombie.generated.h"

UCLASS()
class PLAGUED_ARCADE_API AZombie : public ACharacter
{
	GENERATED_BODY()
	
public:	
	AZombie();

protected:
	virtual void BeginPlay() override;
	void Ragdoll();

public:	
	virtual void Tick(float DeltaTime) override;

	void TakeDamage(int _amount);
	bool IsDead();
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	class UEnemyComponent* EnemyComponent;
};
