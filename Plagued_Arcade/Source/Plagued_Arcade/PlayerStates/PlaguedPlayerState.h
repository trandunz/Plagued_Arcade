#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlaguedPlayerState.generated.h"

UCLASS()
class PLAGUED_ARCADE_API APlaguedPlayerState : public APlayerState
{
	GENERATED_BODY()

	APlaguedPlayerState();
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int CurrentPoints = 500;
};
