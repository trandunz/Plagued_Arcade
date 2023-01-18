#include "MysteryBox.h"

#include "ARTypes.h"
#include "Components/SlateWrapperTypes.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Plagued_Arcade/Plagued_ArcadeCharacter.h"
#include "Plagued_Arcade/Plagued_ArcadeGameMode.h"
#include "InteractComponent.h"
#include "Plagued_Arcade/PlayerStates/PlaguedPlayerState.h"


AMysteryBox::AMysteryBox()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	Hinge = CreateDefaultSubobject<USceneComponent>(TEXT("Hinge"));
	Hinge->SetupAttachment(Mesh);
	Hinge->SetRelativeLocation({0, -20, 30});

	Lid = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	Lid->SetupAttachment(Hinge);
	Lid->SetRelativeLocation({0, 20, -30});

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(Mesh);
	Weapon->SetRelativeLocation({0,0,10});
	Weapon->SetRelativeRotation({0,90,0});

	InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("Interact Component"));
}

void AMysteryBox::BeginPlay()
{
	Super::BeginPlay();

	if (OpenCurve)
	{
		FOnTimelineFloat progress;
		progress.BindUFunction(this, FName("TimelineProgress"));
		FOnTimelineFloat waitProgress;
		waitProgress.BindUFunction(this, FName("WaitTimelineProgress"));
		FOnTimelineFloat closeProgress;
		closeProgress.BindUFunction(this, FName("CloseTimelineProgress"));
		
		OpenTimeline.AddInterpFloat(OpenCurve, progress);
		
		WaitTimeline.AddInterpFloat(OpenCurve, waitProgress);
		WaitTimeline.SetPlayRate(1 / OpenDuration);

		CloseTimeline.AddInterpFloat(OpenCurve, closeProgress);
	}
	if (WeaponCurve)
	{
		FOnTimelineFloat upProgress;
		upProgress.BindUFunction(this, FName("WeaponUpTimelineProgress"));

		FOnTimelineFloat downProgress;
		downProgress.BindUFunction(this, FName("WeaponDownTimelineProgress"));

		WeaponUpTimeline.AddInterpFloat(WeaponCurve, upProgress);
		WeaponDownTimeline.AddInterpFloat(WeaponCurve, downProgress);
	}
}

void AMysteryBox::TimelineProgress(float _value)
{
	Hinge->SetRelativeRotation(FMath::Lerp(FRotator{0,0,0}, FRotator{0,0,-100}, _value));
	
	if (_value >= 1)
	{
		WaitTimeline.PlayFromStart();
		OpenTimeline.Stop();
	}
}

void AMysteryBox::WaitTimelineProgress(float _value)
{
	if (_value >= 0.5f && !WeaponDownTimeline.IsPlaying())
	{
		WeaponDownTimeline.PlayFromStart();
	}
}

void AMysteryBox::CloseTimelineProgress(float _value)
{	
	if (_value >= 1)
	{
		IsOpen = false;
		IsGunReady = false;
		CloseTimeline.Stop();
		WaitTimeline.Stop();
		OpenTimeline.Stop();
		WeaponDownTimeline.Stop();
	}
}

// Add % chance to gun showing up
void AMysteryBox::WeaponUpTimelineProgress(float _value)
{
	Weapon->SetRelativeLocation(FMath::Lerp(FVector{0,0,10}, FVector{0,0,70}, _value));

	if (WeaponChangeTimer <= 0)
	{
		if (APlagued_ArcadeGameMode* gameMode = GetWorld()->GetAuthGameMode<APlagued_ArcadeGameMode>())
		{
			TArray<FWeaponStruct> guns = gameMode->WeaponList;
			for(int i = 0; i < guns.Num(); i++)
			{
				if (guns[i].Mesh == Weapon->GetSkeletalMeshAsset())
				{
					guns.RemoveAt(i);
				}
			}
			ChosenWeapon = guns[rand() % guns.Num()];
			Weapon->SetSkeletalMesh(ChosenWeapon.Mesh);
			
		}

		WeaponChangeTimer = WeaponChangeSpeed;
	}
	else
	{
		WeaponChangeTimer -= _value - CachedValue;
	}

	CachedValue = _value;

	if (_value >= 1)
		IsGunReady = true;
		
}

void AMysteryBox::WeaponDownTimelineProgress(float _value)
{
	
	Weapon->SetRelativeLocation(FMath::Lerp(FVector{0,0,70}, FVector{0,0,10}, _value));

	if (_value >= 0.8f && !CloseTimeline.IsPlaying())
	{
		OpenTimeline.ReverseFromEnd();
		CloseTimeline.PlayFromStart();
		WaitTimeline.Stop();
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), CloseSound, GetActorLocation());
	}
}

void AMysteryBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	OpenTimeline.TickTimeline(DeltaTime);
	WaitTimeline.TickTimeline(DeltaTime);
	CloseTimeline.TickTimeline(DeltaTime);
	WeaponUpTimeline.TickTimeline(DeltaTime);
	WeaponDownTimeline.TickTimeline(DeltaTime);
}

void AMysteryBox::Interact(APlaguedPlayerState* _playerState)
{
	if (!IsOpen && _playerState->CurrentPoints >= GetInteractCost())
	{
		_playerState->CurrentPoints -= GetInteractCost();
		
		IsOpen = true;
		Weapon->SetVisibility(true);
		OpenTimeline.PlayFromStart();
		WeaponUpTimeline.PlayFromStart();
		WeaponChangeTimer = 0.0f;
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), JingleSound, GetActorLocation());
	}
	else if (IsGunReady)
	{
		// Give player the weapon
		OpenTimeline.ReverseFromEnd();
		CloseTimeline.PlayFromStart();
		WaitTimeline.Stop();
		IsGunReady = false;
		Weapon->SetVisibility(false);
		if (APlagued_ArcadeCharacter* character = Cast<APlagued_ArcadeCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()))
		{
			character->EquipWeapon(ChosenWeapon);
		}
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), CloseSound, GetActorLocation());
	}
}

int AMysteryBox::GetInteractCost()
{
	if (InteractComponent)
	{
		return InteractComponent->InteractCost;
	}

	return {};
}

