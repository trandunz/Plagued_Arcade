// Fill out your copyright notice in the Description page of Project Settings.


#include "M1911.h"

#include "Camera/CameraComponent.h"
#include "Plagued_Arcade/Plagued_ArcadeCharacter.h"
#include "GunComponent.h"
#include "Plagued_Arcade/Enemies/Zombie.h"

AM1911::AM1911()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	GunComponent = CreateDefaultSubobject<UGunComponent>(TEXT("Gun Component"));
}

void AM1911::BeginPlay()
{
	Super::BeginPlay();
}

void AM1911::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FireTimer > 0)
		FireTimer -= DeltaTime;
}

bool AM1911::Fire()
{
	if (FireAnimation && FireTimer <= 0.0f && GunComponent->CurrentAmmo > 0)
	{
		FireTimer = FireRate;
		Mesh->PlayAnimation(FireAnimation, false);
		
		if (APlagued_ArcadeCharacter* playerCharacter = Cast<APlagued_ArcadeCharacter>(GetRootComponent()->GetAttachmentRootActor()))
		{
			FHitResult hitResult;
			FVector start = playerCharacter->GetFPSCamera()->GetComponentLocation();
			FVector end = start + playerCharacter->GetFPSCamera()->GetForwardVector() * 2000;
			FCollisionQueryParams params{};
			GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility, params);
			
			if (AZombie* enemy  = Cast<AZombie>(hitResult.GetActor()))
			{
				UE_LOG(LogTemp, Warning, TEXT("Zombie Hit"));
				playerCharacter->GetPlayerHUD()->FlashHitMarker();
				
				if (hitResult.BoneName == FName("Head"))
				{
					enemy->TakeDamage(Damage * 10);
				}
				else
				{
					enemy->TakeDamage(Damage);
				}
			}
		}

		GunComponent->Fire();
		
		return true;
	}

	return false;
}

bool AM1911::Reload()
{
	if (GunComponent->CurrentAmmo < GunComponent->MaxAmmo)
	{
		GunComponent->Reload();
		return true;
	}
	
	return false;
}

