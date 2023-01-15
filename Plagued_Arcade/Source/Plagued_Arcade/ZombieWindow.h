#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZombieWindow.generated.h"

UCLASS()
class PLAGUED_ARCADE_API AZombieWindow : public AActor
{
	GENERATED_BODY()
	
public:	
	AZombieWindow();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void RemoveBoard();

protected:
	USceneComponent* BoardParent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneComponent* ZombieTarget;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BoardOne;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BoardTwo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BoardThree;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BoardFour;

	float BoardRemoveSpeed = 1.0f;
	float BoardRemoveTimer = 0.0f;
};
