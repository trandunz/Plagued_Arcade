#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLAGUED_ARCADE_API UEnemyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemyComponent();

	void TakeDamage(int _amount);
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxHealth = 100;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int WorthInPoints = 100;
};
