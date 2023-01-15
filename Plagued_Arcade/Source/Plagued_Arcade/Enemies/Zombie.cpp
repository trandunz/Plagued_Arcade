#include "Zombie.h"

#include "EnemyComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AZombie::AZombie()
{
	PrimaryActorTick.bCanEverTick = true;

	EnemyComponent = CreateDefaultSubobject<UEnemyComponent>(TEXT("Enemy Component"));
}

void AZombie::BeginPlay()
{
	Super::BeginPlay();
}

void AZombie::Ragdoll()
{
	SetReplicateMovement(false);

	DetachFromControllerPendingDestroy();
	
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	SetActorEnableCollision(true);
	
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->WakeAllRigidBodies();
	GetMesh()->bBlendPhysics = true;

	UCharacterMovementComponent* CharacterComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if (CharacterComp)
	{
		CharacterComp->StopMovementImmediately();
		CharacterComp->DisableMovement();
		CharacterComp->SetComponentTickEnabled(false);
	}

	SetLifeSpan(10.0f);
}

void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AZombie::TakeDamage(int _amount)
{
	if (EnemyComponent)
	{
		if (EnemyComponent->CurrentHealth > 0)
		{
			EnemyComponent->TakeDamage(_amount);
			if (EnemyComponent->CurrentHealth <= 0)
			{
				Ragdoll();
			}
		}
	}
}

