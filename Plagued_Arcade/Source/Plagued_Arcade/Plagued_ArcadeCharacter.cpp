// Copyright Epic Games, Inc. All Rights Reserved.

#include "Plagued_ArcadeCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Interactables/MysteryBox.h"
#include "Weapons/Guns/GunComponent.h"
#include "InputMappingContext.h"
#include "Plagued_Arcade/Plagued_ArcadeGameMode.h"
#include "InputAction.h"
#include "Interactables/Wallchalk.h"
#include "PlayerStates/PlaguedPlayerState.h"
#include "Weapons/Guns/M1911.h"


//////////////////////////////////////////////////////////////////////////
// APlagued_ArcadeCharacter

APlagued_ArcadeCharacter::APlagued_ArcadeCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = StandardWalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	GetMesh()->SetOwnerNoSee(true);
	
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCam"));
	FirstPersonCamera->SetupAttachment(RootComponent);
	FirstPersonCamera->bUsePawnControlRotation = true;

	FirstPersonArms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Arms"));
	FirstPersonArms->SetupAttachment(FirstPersonCamera);
}

UCameraComponent* APlagued_ArcadeCharacter::GetFPSCamera()
{
	return FirstPersonCamera;
}

UPlayerHUD* APlagued_ArcadeCharacter::GetPlayerHUD()
{
	return PlayerHUD;
}

void APlagued_ArcadeCharacter::EquipWeapon(FWeaponStruct& _weapon)
{
	DestroyHeldWeapon();
	CurrentWeapon = GetWorld()->SpawnActor(_weapon.Prefab);
	CurrentWeapon->AttachToComponent(FirstPersonArms, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("PistolSocket"));	
}

void APlagued_ArcadeCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (IsLocallyControlled())
	{
		if (PlayerHudBP)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player Hud Created"));
			APlayerController* controller = GetController<APlayerController>();
			PlayerHUD = CreateWidget<UPlayerHUD>(controller, PlayerHudBP);
			PlayerHUD->AddToPlayerScreen();
		}
	}
	if (M1911_BP)
	{
		CurrentWeapon = GetWorld()->SpawnActor<AM1911>(M1911_BP);
		CurrentWeapon->AttachToComponent(FirstPersonArms, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("PistolSocket"));	
	}
}

void APlagued_ArcadeCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (PlayerHUD)
	{
		PlayerHUD->RemoveFromParent();
		PlayerHUD = nullptr;
	}
}

void APlagued_ArcadeCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (PlayerHUD)
	{
		if (APlaguedPlayerState* playerState = GetPlayerState<APlaguedPlayerState>())
		{
			PlayerHUD->UpdatePointsText(playerState->CurrentPoints);
		}
		
		if (CurrentWeapon)
		{
			if (UGunComponent* gun = Cast<UGunComponent>(CurrentWeapon->FindComponentByClass<UGunComponent>()))
			{
				PlayerHUD->UpdateAmmoCount(gun->CurrentAmmo, gun->MaxAmmo);
			}
			else
			{
				PlayerHUD->UpdateAmmoCount(0,0);
			}
		}
		else
		{
			PlayerHUD->UpdateAmmoCount(0,0);
		}
	}

	InteractLinetrace();
}

void APlagued_ArcadeCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlagued_ArcadeCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlagued_ArcadeCharacter::Look);

		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &APlagued_ArcadeCharacter::Aim);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &APlagued_ArcadeCharacter::StopAim);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &APlagued_ArcadeCharacter::Fire);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &APlagued_ArcadeCharacter::Reload);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &APlagued_ArcadeCharacter::TryInteract);
	}
}

void APlagued_ArcadeCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void APlagued_ArcadeCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(-LookAxisVector.Y);
	}
}

void APlagued_ArcadeCharacter::Aim()
{
	IsAiming = true;
	GetCharacterMovement()->MaxWalkSpeed = AimWalkSpeed;
}

void APlagued_ArcadeCharacter::StopAim()
{
	IsAiming = false;
	GetCharacterMovement()->MaxWalkSpeed = StandardWalkSpeed;
}

void APlagued_ArcadeCharacter::Fire()
{
	if (IsAiming)
	{
		if (IGunInterface* gun = Cast<IGunInterface>(CurrentWeapon))
		{
			if (gun->Fire())
			{
				UAnimInstance* AnimInstance = FirstPersonArms->GetAnimInstance();
				if (AnimInstance)
				{
					AnimInstance->Montage_Play(AimFireMontage);
				}
			}
		}
	}
	else
	{
		if (IGunInterface* gun = Cast<IGunInterface>(CurrentWeapon))
		{
			if (gun->Fire())
			{
				UAnimInstance* AnimInstance = FirstPersonArms->GetAnimInstance();
				if (AnimInstance)
				{
					AnimInstance->Montage_Play(HipFireMontage);
				}
			}
		}
	}
}

void APlagued_ArcadeCharacter::Reload()
{
	if (IGunInterface* gun = Cast<IGunInterface>(CurrentWeapon))
	{
		if (gun->Reload())
		{
			
		}
	}
}

void APlagued_ArcadeCharacter::TryInteract()
{
	if (IInteractInterface* interactable = Cast<IInteractInterface>(LastHitResult.GetActor()))
	{
		interactable->Interact(GetPlayerState<APlaguedPlayerState>());
	}
}

void APlagued_ArcadeCharacter::InteractLinetrace()
{
	FVector start = GetFPSCamera()->GetComponentLocation();
	FVector end = start + GetFPSCamera()->GetForwardVector() * 200;
	FCollisionQueryParams params{};
	GetWorld()->LineTraceSingleByChannel(LastHitResult, start, end, ECC_Visibility, params);
	
	if (PlayerHUD)
	{
		PlayerHUD->UpdateInteractText("", "");

		if (AMysteryBox* mysteryBox  = Cast<AMysteryBox>(LastHitResult.GetActor()))
		{
			if (!mysteryBox->IsOpen)
			{
				PlayerHUD->UpdateInteractText(GetKeyFromInputAction(InteractAction).ToString(), "To Open Box (" + FString::FromInt(mysteryBox->GetInteractCost()) + ")");
			}
			else if (mysteryBox->IsGunReady)
			{
				PlayerHUD->UpdateInteractText(GetKeyFromInputAction(InteractAction).ToString(), "To Pickup " + mysteryBox->ChosenWeapon.Name);
			}
		}
		else if (AWallchalk* wallchalk = Cast<AWallchalk>(LastHitResult.GetActor()))
		{
			PlayerHUD->UpdateInteractText(GetKeyFromInputAction(InteractAction).ToString(), "To Buy " + wallchalk->Weapon.Name + "(" + FString::FromInt(wallchalk->InteractComponent->InteractCost) + ")");
		}
	}
}

void APlagued_ArcadeCharacter::DestroyHeldWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}
}

FKey APlagued_ArcadeCharacter::GetKeyFromInputAction(UInputAction* _action) const
{
	if (DefaultMappingContext)
	{
		for(auto& mapping : DefaultMappingContext->GetMappings())
		{
			if (mapping.Action == _action)
			{
				return mapping.Key;
			}
		}
	}
	return {};
}




