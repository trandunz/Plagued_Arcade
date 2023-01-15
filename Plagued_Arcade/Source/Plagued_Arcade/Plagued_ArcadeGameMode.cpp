// Copyright Epic Games, Inc. All Rights Reserved.

#include "Plagued_ArcadeGameMode.h"
#include "Plagued_ArcadeCharacter.h"
#include "UObject/ConstructorHelpers.h"

APlagued_ArcadeGameMode::APlagued_ArcadeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_PlaguedCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
