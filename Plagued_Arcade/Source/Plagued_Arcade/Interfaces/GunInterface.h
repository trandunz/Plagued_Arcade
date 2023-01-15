// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GunInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGunInterface : public UInterface
{
	GENERATED_BODY()
};

class PLAGUED_ARCADE_API IGunInterface
{
	GENERATED_BODY()

public:
	virtual bool Fire() = 0;
	virtual bool Reload() = 0;
};
