#include "Plagued_ArcadeGameMode.h"
#include "Plagued_ArcadeCharacter.h"
#include "Enemies/Zombie.h"
#include "GameStates/PlaguedGameState.h"
#include "Sound/SoundCue.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerStates/PlaguedPlayerState.h"

APlagued_ArcadeGameMode::APlagued_ArcadeGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_PlaguedCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void APlagued_ArcadeGameMode::BeginPlay()
{
	Super::BeginPlay();

	CurrentRound = 0;
	RoundIntervalTimer = 1.0f;
}

void APlagued_ArcadeGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AZombie::StaticClass(), FoundActors);
	NumberOfAliveZombies = 0;
	for(auto zombie : FoundActors)
	{
		if (!Cast<AZombie>(zombie)->IsDead())
		{
			NumberOfAliveZombies++;
		}
	}
	
	if (RoundIntervalTimer > 0 && NumberOfAliveZombies <= 0)
	{
		RoundIntervalTimer -= DeltaSeconds;
	}
	else if (CurrentZombieCount <= 0 && RoundIntervalTimer <= 0)
	{
		if (NumberOfAliveZombies <= 0)
		{
			ChangeRound();
		}
	}
}

void APlagued_ArcadeGameMode::ChangeRound()
{
	CurrentRound++;
	UGameplayStatics::PlaySound2D(GetWorld(), RoundStartSound);

	if (CurrentRound == 1)
	{
		CurrentZombieCount = 4;
	}
	else
	{
		CurrentZombieCount = 4 + (5 * (CurrentRound - 1));
	}

	RoundIntervalTimer = TimeBetweenRounds;

	if (APlagued_ArcadeCharacter* character = Cast<APlagued_ArcadeCharacter>(GetWorld()->GetFirstLocalPlayerFromController()->PlayerController->GetCharacter()))
	{
		if (character->GetPlayerHUD())
		{
			character->GetPlayerHUD()->UpdateRoundText(CurrentRound);
		}
	}
}

void APlagued_ArcadeGameMode::PlayRoundEndSound()
{
	UGameplayStatics::PlaySound2D(GetWorld(), RoundEndSound);
}

int APlagued_ArcadeGameMode::GetZombiesThisRound()
{
	if (CurrentRound == 1)
	{
		return 4;
	}
	else
	{
		return 4 + (5 * (CurrentRound - 1));
	}
}
