#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Plagued_ArcadeGameMode.generated.h"

USTRUCT(BlueprintType)
struct FWeaponStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> Prefab;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture* WallChalkTexture;

	bool operator == (FWeaponStruct &rhs) const
	{
		if (Mesh == rhs.Mesh)
			return true;
		else
			return false;
	}
};

UCLASS(minimalapi)
class APlagued_ArcadeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APlagued_ArcadeGameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	void ChangeRound();
	void PlayRoundEndSound();
	int GetZombiesThisRound();

	float RoundIntervalTimer = 0.0f;
	float TimeBetweenRounds = 9.0f;
	
	int CurrentRound = 0;

	int NumberOfAliveZombies = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int CurrentZombieCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USoundCue* RoundStartSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USoundCue* RoundEndSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FWeaponStruct> WeaponList;
};



