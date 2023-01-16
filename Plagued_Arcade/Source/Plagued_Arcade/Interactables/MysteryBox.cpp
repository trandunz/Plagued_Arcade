#include "MysteryBox.h"

#include "ARTypes.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"


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

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(Mesh);
	Weapon->SetRelativeLocation({0,0,10});
	Weapon->SetRelativeRotation({0,90,0});
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
		CloseTimeline.Stop();
		WaitTimeline.Stop();
		OpenTimeline.Stop();
		WeaponDownTimeline.Stop();
	}
}

void AMysteryBox::WeaponUpTimelineProgress(float _value)
{
	Weapon->SetRelativeLocation(FMath::Lerp(FVector{0,0,10}, FVector{0,0,70}, _value));
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

void AMysteryBox::Interact()
{
	if (!IsOpen)
	{
		IsOpen = true;
		OpenTimeline.PlayFromStart();
		WeaponUpTimeline.PlayFromStart();
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), JingleSound, GetActorLocation());
	}
}

