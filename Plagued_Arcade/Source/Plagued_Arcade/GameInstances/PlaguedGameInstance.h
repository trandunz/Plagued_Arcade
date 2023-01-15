// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Engine/GameInstance.h"
#include "PlaguedGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PLAGUED_ARCADE_API UPlaguedGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPlaguedGameInstance();

protected:
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	IOnlineSessionPtr SessionInterface;

	virtual void Init() override;
	virtual void OnCreateSessionComplete(FName _sessionName, bool _succeeded);
	virtual void OnFindSessionComplete(bool _succeeded);
	virtual void OnJoinSessionComplete(FName _sessionName, EOnJoinSessionCompleteResult::Type _result);

	UFUNCTION(BlueprintCallable)
	void CreateServer();
	UFUNCTION(BlueprintCallable)
	void JoinServer();
};
