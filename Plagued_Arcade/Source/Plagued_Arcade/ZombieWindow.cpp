#include "ZombieWindow.h"
#include "Components/StaticMeshComponent.h"

AZombieWindow::AZombieWindow()
{
	PrimaryActorTick.bCanEverTick = true;

	BoardParent = CreateDefaultSubobject<USceneComponent>(TEXT("Board_Parent"));
	BoardParent->SetupAttachment(RootComponent);

	ZombieTarget = CreateDefaultSubobject<USceneComponent>(TEXT("Zombie_Target"));
	ZombieTarget->SetupAttachment(BoardParent);
	
	BoardOne = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Board_One"));
	BoardTwo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Board_Two"));
	BoardThree = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Board_Three"));
	BoardFour = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Board_Four"));

	BoardOne->SetupAttachment(BoardParent);
	BoardTwo->SetupAttachment(BoardParent);
	BoardThree->SetupAttachment(BoardParent);
	BoardFour->SetupAttachment(BoardParent);

	BoardRemoveTimer = BoardRemoveSpeed;
}

void AZombieWindow::BeginPlay()
{
	Super::BeginPlay();
	
}

void AZombieWindow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BoardRemoveTimer > 0)
		BoardRemoveTimer -= DeltaTime;
	else
	{
		BoardRemoveTimer = BoardRemoveSpeed;
		RemoveBoard();
	}
}

void AZombieWindow::RemoveBoard()
{
	TArray<UStaticMeshComponent*> availableBoards;
	if (BoardOne)
	{
		availableBoards.Add(BoardOne);
	}
	if (BoardTwo)
	{
		availableBoards.Add(BoardTwo);
	}
	if (BoardThree)
	{
		availableBoards.Add(BoardThree);
	}
	if (BoardFour)
	{
		availableBoards.Add(BoardFour);
	}

	if (availableBoards.Num() > 0)
	{
		int randomBoard = rand() % availableBoards.Num();
        availableBoards[randomBoard]->DestroyComponent();
	}
}

