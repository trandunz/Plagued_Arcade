#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GunComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLAGUED_ARCADE_API UGunComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGunComponent();

	void Reload();
	void Fire();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int CurrentAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxAmmo = 10;
};
