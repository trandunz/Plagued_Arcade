#pragma once

#include "CoreMinimal.h"
#include "InteractComponent.h"
#include "GameFramework/Actor.h"
#include "Plagued_Arcade/Plagued_ArcadeGameMode.h"
#include "Plagued_Arcade/Interfaces/InteractInterface.h"
#include "Wallchalk.generated.h"

UCLASS()
class PLAGUED_ARCADE_API AWallchalk : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	AWallchalk();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(APlaguedPlayerState* _playerState) override;

	int GetInteractCost();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Chalk;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FWeaponStruct Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInteractComponent* InteractComponent;
};
