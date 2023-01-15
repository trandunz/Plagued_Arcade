#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Plagued_Arcade/Interfaces/GunInterface.h"
#include "M1911.generated.h"

UCLASS()
class PLAGUED_ARCADE_API AM1911 : public AActor, public IGunInterface
{
	GENERATED_BODY()
	
public:	
	AM1911();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	bool Fire() override;
	bool Reload() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UAnimationAsset* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Damage = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FireRate = 0.3f;
	float FireTimer = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UGunComponent* GunComponent;
};
