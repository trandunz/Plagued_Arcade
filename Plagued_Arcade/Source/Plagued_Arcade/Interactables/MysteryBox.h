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

	bool IsOpen = false;

public:	
	virtual void Tick(float DeltaTime) override;
	void Interact();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Lid;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USceneComponent* Hinge;

	FTimeline OpenTimeline;
	FTimeline WaitTimeline;
	FTimeline CloseTimeline;
	UPROPERTY(EditAnywhere, Category = Timeline)
	class UCurveFloat* OpenCurve;

	UPROPERTY(EditAnywhere, Category = BoxSettings)
	float OpenDuration = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USoundWave* OpenSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USoundWave* JingleSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USoundWave* CloseSound;
};
