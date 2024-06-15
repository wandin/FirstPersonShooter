// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "FirstPersonShooter/PlayerController/FPSPlayerController.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

AFPSCharacter::AFPSCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//SpringArm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocation(FVector(0.f,0.f,90.f));
	SpringArm->TargetArmLength = 600.f;
	SpringArm->bUsePawnControlRotation = true;
	//Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	CheckPlayerControllerAndSetInputMappings();
	
}

void AFPSCharacter::CheckPlayerControllerAndSetInputMappings()
{
	AController* LocalController = GetController();

	if(LocalController && LocalController->GetClass()->ImplementsInterface(UPlayerControllerInterface::StaticClass()))
	{
		IPlayerControllerInterface* PCInterface = Cast<IPlayerControllerInterface>(LocalController);
		if(PCInterface)
		{
			APlayerController* PC = PCInterface->GetPlayerControllerRef();
			if(PC)
			{
				/* Mapping Input*/
				if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
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

}

void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		//Move
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFPSCharacter::Move);
		// Walk (not running / no footstep sounds)
		/*
		EIC->BindAction(WalkAction, ETriggerEvent::Triggered, this, &ADFNCharacter::WalkPressed);
		EIC->BindAction(WalkAction, ETriggerEvent::Completed, this, &ADFNCharacter::WalkReleased);
		*/

		EIC->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AFPSCharacter::CrouchPressed);
		EIC->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AFPSCharacter::CrouchReleased);
		//Look
		EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFPSCharacter::Look);
	}

}

ACharacter* AFPSCharacter::GetCharacterRef()
{
	return this;
}

void AFPSCharacter::Move(const FInputActionValue& Value)
{
	const FVector2d MovementVector = Value.Get<FVector2d>();
	AddMovementInput(GetActorForwardVector(), MovementVector.X);	// we get the Axis (X) from ForwardVector
	AddMovementInput(GetActorRightVector(), MovementVector.Y);		// we get the Axis (Y) from RightVector
}

void AFPSCharacter::Look(const FInputActionValue& Value)
{
	const FVector2d LookAxisVector = Value.Get<FVector2d>();
	if(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->AddPitchInput(LookAxisVector.Y);
	}
	AddControllerYawInput(LookAxisVector.X);
}

void AFPSCharacter::CrouchPressed()
{
	if(!bIsCrouched)
	{
		Crouch();
		bIsCrouched = true;
	}
}

void AFPSCharacter::CrouchReleased()
{
	if(bIsCrouched)
	{
		UnCrouch();
		bIsCrouched = false;
	}
}