// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaguedGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

UPlaguedGameInstance::UPlaguedGameInstance()
{
}

void UPlaguedGameInstance::Init()
{
	Super::Init();

	if (IOnlineSubsystem* subSystem = IOnlineSubsystem::Get())
	{
		SessionInterface = subSystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPlaguedGameInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPlaguedGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPlaguedGameInstance::OnJoinSessionComplete);
		}
	}
}

void UPlaguedGameInstance::OnCreateSessionComplete(FName _sessionName, bool _succeeded)
{
	if (_succeeded)
	{
		GetWorld()->ServerTravel("/Game/Maps/ThirdPersonMap?listen");
	}
}

void UPlaguedGameInstance::OnFindSessionComplete(bool _succeeded)
{
	if (_succeeded)
	{
		TArray<FOnlineSessionSearchResult> searchResult = SessionSearch->SearchResults;
		if (searchResult.Num() > 0)
		{
			SessionInterface->JoinSession(0, FName("Plagued Session"), searchResult[0]);
		}
	}
}

void UPlaguedGameInstance::OnJoinSessionComplete(FName _sessionName, EOnJoinSessionCompleteResult::Type _result)
{
	if (APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FString joinAddress = "";
		SessionInterface->GetResolvedConnectString(_sessionName, joinAddress);
		if (joinAddress != "")
		{
			controller->ClientTravel(joinAddress,TRAVEL_Absolute);
		}
	}
}

void UPlaguedGameInstance::CreateServer()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings sessionSettings;
		sessionSettings.bAllowJoinInProgress = true;
		sessionSettings.bIsDedicated = false;
		sessionSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() != "NULL" ? false : true;
		sessionSettings.bShouldAdvertise = true;
		sessionSettings.bUsesPresence = true;
		sessionSettings.NumPublicConnections = 5;

		SessionInterface->CreateSession(0, FName("Plagued Session"), sessionSettings);
	}
}

void UPlaguedGameInstance::JoinServer()
{
	if (SessionInterface.IsValid())
	{
		SessionSearch = MakeShareable(new FOnlineSessionSearch());
		SessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() != "NULL" ? false : true;
		SessionSearch->MaxSearchResults = 10000;
		SessionSearch->QuerySettings.Set("SEARCH_PRESENCE", true, EOnlineComparisonOp::Equals);

		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}
