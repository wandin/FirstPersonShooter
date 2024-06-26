// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "FirstPersonShooter/PlayerController/FPSPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

AFPSCharacter::AFPSCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	ShadowMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShadowMesh"));
	if(ShadowMesh)
	{
		ShadowMesh->SetupAttachment(RootComponent);
		ShadowMesh->bHiddenInGame = true;
		ShadowMesh->SetCastHiddenShadow(true);
	}
	// SpringArm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	if(SpringArm)
	{
		SpringArm->SetupAttachment(GetMesh(), "headSocket"); //attaching to the Head socket
		SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		SpringArm->TargetArmLength = 0.f;
		SpringArm->bUsePawnControlRotation = true;
	}
	// Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	if(Camera)
	{
		Camera->SetupAttachment(SpringArm);
	}

	if(GetCharacterMovement())
	{
		// Walk of Ledges
		GetCharacterMovement()->bCanWalkOffLedges = true;
		GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;
		GetCharacterMovement()->LedgeCheckThreshold = 0.0f;
	}
}

void AFPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSCharacter, bWalking);
	DOREPLIFETIME(AFPSCharacter, AimPitch);
}

void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	CheckPlayerControllerAndSetInputMappings();

	if (IsLocallyControlled())
	{
		// Hide Head bone to not overlap with camera
		GetMesh()->HideBoneByName("head", PBO_None);
	}
}

void AFPSCharacter::CheckPlayerControllerAndSetInputMappings() const
{
	AController* LocalController = GetController();

	if (LocalController && LocalController->GetClass()->ImplementsInterface(UPlayerControllerInterface::StaticClass()))
	{
		if (IPlayerControllerInterface* PCInterface = Cast<IPlayerControllerInterface>(LocalController))
		{
			APlayerController* PC = PCInterface->GetPlayerControllerRef();
			if (PC)
			{
				/* Mapping Input*/
				if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
					UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
				{
					UE_LOG(LogTemp, Warning, TEXT("Beginplay - Subsystem is valid!"));
					Subsystem->AddMappingContext(PlayerMappingContext, 0);
				}
			}
		}
	}
}

void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CalculateAimDirection();
}

void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Setting Inputs
	if (UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		//Look
		EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFPSCharacter::Look);
		//Move
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFPSCharacter::Move);
		// Walk (not running / no footstep sounds)
		EIC->BindAction(WalkAction, ETriggerEvent::Triggered, this, &AFPSCharacter::WalkPressed);
		EIC->BindAction(WalkAction, ETriggerEvent::Completed, this, &AFPSCharacter::WalkReleased);
		// Crouch
		EIC->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AFPSCharacter::CrouchPressed);
		EIC->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AFPSCharacter::CrouchReleased);
		//Jump
		EIC->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AFPSCharacter::Jump);
		EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &AFPSCharacter::StopJumping);
	}
}

AFPSCharacter* AFPSCharacter::GetCharacterRef()
{
	return this;
}

void AFPSCharacter::CalculateAimDirection()
{
	if (HasAuthority())
	{
		Server_CalculateAimDirection();
	}
	FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(GetControlRotation(), GetActorRotation());
	AimPitch = DeltaRotation.Pitch;
}

void AFPSCharacter::Server_CalculateAimDirection_Implementation()
{
	FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(GetControlRotation(), GetActorRotation());
	AimPitch = DeltaRotation.Pitch;
	Multicast_UpdateAimPitch(AimPitch); // Chama o Multicast para atualizar AimPitch em todos os clientes

}
void AFPSCharacter::Multicast_UpdateAimPitch_Implementation(float NewAimPitch)
{
	AimPitch = NewAimPitch;
}

void AFPSCharacter::Move(const FInputActionValue& Value)
{
	const FVector2d MovementVector = Value.Get<FVector2d>();
	AddMovementInput(GetActorForwardVector(), MovementVector.X); // we get the Axis (X) from ForwardVector
	AddMovementInput(GetActorRightVector(), MovementVector.Y); // we get the Axis (Y) from RightVector
}

void AFPSCharacter::Look(const FInputActionValue& Value)
{
	const FVector2d LookAxisVector = Value.Get<FVector2d>();
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->AddPitchInput(LookAxisVector.Y);
	}
	AddControllerYawInput(LookAxisVector.X);
}

void AFPSCharacter::CrouchPressed()
{
	if(!GetCharacterMovement()->IsFalling() && !bIsCrouched)
	{
		Crouch();
	}
	else if (GetCharacterMovement()->IsFalling() && bIsCrouched)
	{
		Crouch();
	}
}

void AFPSCharacter::CrouchReleased()
{
	if(bIsCrouched)
	{
		UnCrouch();
	}
}

void AFPSCharacter::WalkPressed()
{
	if (!GetCharacterMovement()) return;
	float WalkSpeed = GetCharacterMovement()->MaxWalkSpeedCrouched;
	if (IsLocallyControlled())
	{
		Server_WalkPressed();
	}
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	bWalking = true;
}

void AFPSCharacter::WalkReleased()
{
	if (!GetCharacterMovement()) return;
	if (IsLocallyControlled())
	{
		Server_WalkReleased();
	}
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	bWalking = false;
}

void AFPSCharacter::Server_WalkPressed_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	bWalking = true;
}

void AFPSCharacter::Server_WalkReleased_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	bWalking = false;
}

void AFPSCharacter::Jump()
{
	if(bIsCrouched)
	{
		UnCrouch();		
	}
	Super::Jump();
}

void AFPSCharacter::StopJumping()
{
	Super::StopJumping();
}
