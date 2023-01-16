#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "MysteryBox.generated.h"

UCLASS()
class PLAGUED_ARCADE_API AMysteryBox : public AActor
{
	GENERATED_BODY()
	
public:	
	AMysteryBox();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void TimelineProgress(float _value);
	UFUNCTION()
	void WaitTimelineProgress(float _value);
	UFUNCTION()
	void CloseTimelineProgress(float _value);

	UFUNCTION()
	void WeaponUpTimelineProgress(float _value);
	UFUNCTION()
	void WeaponDownTimelineProgress(float _value);

public:	
	virtual void Tick(float DeltaTime) override;
	void Interact();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Lid;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USceneComponent* Hinge;

	FTimeline OpenTimeline;
	FTimeline WaitTimeline;
	FTimeline CloseTimeline;

	FTimeline WeaponUpTimeline;
	FTimeline WeaponDownTimeline;
	
	UPROPERTY(EditAnywhere, Category = Timeline)
	class UCurveFloat* OpenCurve;

	UPROPERTY(EditAnywhere, Category = Timeline)
	class UCurveFloat* WeaponCurve;

	UPROPERTY(EditAnywhere, Category = BoxSettings)
	float OpenDuration = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USoundWave* OpenSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USoundWave* JingleSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USoundWave* CloseSound;

public:
	bool IsOpen = false;
};
