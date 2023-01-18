#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Plagued_Arcade/Interfaces/InteractInterface.h"
#include "InteractComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLAGUED_ARCADE_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int InteractCost = 950;
};
