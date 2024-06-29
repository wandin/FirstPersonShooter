// Fill out your copyright notice in the Description page of Project Settings.


#include "WDNCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "WeDefeatNations/PlayerController/WDNPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

AWDNCharacter::AWDNCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	MovementComponent = GetCharacterMovement();

	CharacterMesh = GetMesh();
	CharacterMesh->CastShadow = false;
	
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

	if(MovementComponent)
	{
		// Walk of Ledges
		MovementComponent->bCanWalkOffLedges = true;
		MovementComponent->bCanWalkOffLedgesWhenCrouching = true;
		MovementComponent->LedgeCheckThreshold = 0.0f;
	}
}

void AWDNCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWDNCharacter, bWalking);
	DOREPLIFETIME(AWDNCharacter, AimPitch);
}

void AWDNCharacter::BeginPlay()
{
	Super::BeginPlay();

	CheckPlayerControllerAndSetInputMappings();

	if (IsLocallyControlled() && GetMesh())
	{
		// Hide Head bone to not overlap with camera
		GetMesh()->HideBoneByName("head", PBO_None);
	}
}

void AWDNCharacter::CheckPlayerControllerAndSetInputMappings() const
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
					Subsystem->AddMappingContext(PlayerMappingContext, 0);
				}
			}
		}
	}
}

void AWDNCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CalculateAimDirection();
}

void AWDNCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Setting Inputs
	if (UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		//Look
		EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWDNCharacter::Look);
		//Move
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWDNCharacter::Move);
		// Walk (not running / no footstep sounds)
		EIC->BindAction(WalkAction, ETriggerEvent::Triggered, this, &AWDNCharacter::WalkPressed);
		EIC->BindAction(WalkAction, ETriggerEvent::Completed, this, &AWDNCharacter::WalkReleased);
		// Crouch
		EIC->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AWDNCharacter::CrouchPressed);
		EIC->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AWDNCharacter::CrouchReleased);
		//Jump
		EIC->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AWDNCharacter::Jump);
		EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &AWDNCharacter::StopJumping);
	}
}

AWDNCharacter* AWDNCharacter::GetCharacterRef()
{
	return this;
}

void AWDNCharacter::CalculateAimDirection()
{
	if (HasAuthority())
	{
		Server_CalculateAimDirection();
	}
	FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(GetControlRotation(), GetActorRotation());
	AimPitch = DeltaRotation.Pitch;
}

void AWDNCharacter::Server_CalculateAimDirection_Implementation()
{
	FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(GetControlRotation(), GetActorRotation());
	AimPitch = DeltaRotation.Pitch;
	Multicast_UpdateAimPitch(AimPitch); // Chama o Multicast para atualizar AimPitch em todos os clientes

}
void AWDNCharacter::Multicast_UpdateAimPitch_Implementation(float NewAimPitch)
{
	AimPitch = NewAimPitch;
}

void AWDNCharacter::Move(const FInputActionValue& Value)
{
	const FVector2d MovementVector = Value.Get<FVector2d>();
	AddMovementInput(GetActorForwardVector(), MovementVector.X); // we get the Axis (X) from ForwardVector
	AddMovementInput(GetActorRightVector(), MovementVector.Y); // we get the Axis (Y) from RightVector
}

void AWDNCharacter::Look(const FInputActionValue& Value)
{
	const FVector2d LookAxisVector = Value.Get<FVector2d>();
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->AddPitchInput(LookAxisVector.Y);
	}
	AddControllerYawInput(LookAxisVector.X);
}

void AWDNCharacter::WalkPressed()
{
	if (!MovementComponent) return;
	float WalkSpeed = MovementComponent->MaxWalkSpeedCrouched;
	if (IsLocallyControlled())
	{
		Server_WalkPressed();
	}
	MovementComponent->MaxWalkSpeed = WalkSpeed;
	bWalking = true;
}

void AWDNCharacter::WalkReleased()
{
	if (!MovementComponent) return;
	if (IsLocallyControlled())
	{
		Server_WalkReleased();
	}
	MovementComponent->MaxWalkSpeed = 600.f;
	bWalking = false;
}

void AWDNCharacter::Server_WalkPressed_Implementation()
{
	MovementComponent->MaxWalkSpeed = 300.f;
	bWalking = true;
}

void AWDNCharacter::Server_WalkReleased_Implementation()
{
	MovementComponent->MaxWalkSpeed = 600.f;
	bWalking = false;
}

void AWDNCharacter::CrouchPressed()
{
	Crouch();
}

void AWDNCharacter::CrouchReleased()
{
	UnCrouch();
}

bool AWDNCharacter::CanJumpInternal_Implementation() const
{
	if(bIsCrouched && !MovementComponent->IsFalling())
	{
		return true;
	}
	return Super::CanJumpInternal_Implementation();
}

void AWDNCharacter::Jump()
{
	if(CanJumpInternal())
	{
		if(JumpLandingSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, JumpLandingSound, GetActorLocation());
		}
		Super::Jump();
	}
}

void AWDNCharacter::StopJumping()
{
	Super::StopJumping();
}

void AWDNCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	if(JumpLandingSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, JumpLandingSound, GetActorLocation());
	}
}