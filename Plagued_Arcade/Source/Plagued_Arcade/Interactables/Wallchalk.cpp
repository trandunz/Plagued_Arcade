#include "Wallchalk.h"

#include "Plagued_Arcade/Plagued_ArcadeCharacter.h"
#include "Plagued_Arcade/Plagued_ArcadeGameMode.h"
#include "Plagued_Arcade/PlayerStates/PlaguedPlayerState.h"

AWallchalk::AWallchalk()
{
	PrimaryActorTick.bCanEverTick = true;


	Chalk = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chalk"));
	Chalk->SetupAttachment(Mesh);
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("Interact Component"));
}

void AWallchalk::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWallchalk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWallchalk::Interact(APlaguedPlayerState* _playerState)
{
	if (_playerState->CurrentPoints >= GetInteractCost())
	{
		if (APlagued_ArcadeCharacter* character = Cast<APlagued_ArcadeCharacter>(GetWorld()->GetFirstLocalPlayerFromController()->PlayerController->GetCharacter()))
		{
			character->EquipWeapon(Weapon);
			_playerState->CurrentPoints -= GetInteractCost();
		
		}
	}
}

int AWallchalk::GetInteractCost()
{
	if (InteractComponent)
	{
		return InteractComponent->InteractCost;
	}

	return {};
}

