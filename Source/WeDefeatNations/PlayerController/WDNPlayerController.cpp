// Fill out your copyright notice in the Description page of Project Settings.


#include "WDNPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "WeDefeatNations/Character/WDNCharacter.h"
#include "WeDefeatNations/UI/WDNHUD.h"

class UEnhancedInputLocalPlayerSubsystem;

void AWDNPlayerController::BeginPlay()
{
	Super::BeginPlay();

	GameHUD = Cast<AWDNHUD>(GetHUD());

	if(GameHUD)
	{
		//GameHUD->AddTeamSelectionWidget();
	}

	WDNCharacter = GetCharacterRef();
	if(WDNCharacter)
	{
		// Log or perform any necessary initialization here
		UE_LOG(LogTemp, Log, TEXT("PlayerController now possesses: %s"), *WDNCharacter->GetName());
		
		// Check and set input mappings
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}
}

APlayerController* AWDNPlayerController::GetPlayerControllerRef()
{
	return this;
}

AWDNCharacter* AWDNPlayerController::GetCharacterRef()
{
	if (APawn* MyPawn = GetPawn())
	{
		return Cast<AWDNCharacter>(MyPawn);
	}
	return nullptr;
}

void AWDNPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Bind inputs
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWDNPlayerController::Move);
		EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWDNPlayerController::Look);
		EIC->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AWDNPlayerController::CrouchPressed);
		EIC->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AWDNPlayerController::CrouchReleased);
		EIC->BindAction(WalkAction, ETriggerEvent::Triggered, this, &AWDNPlayerController::WalkPressed);
		EIC->BindAction(WalkAction, ETriggerEvent::Completed, this, &AWDNPlayerController::WalkReleased);
		EIC->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AWDNPlayerController::StartJump);
		EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &AWDNPlayerController::StopJump);
	}
}

void AWDNPlayerController::Move(const FInputActionValue& Value)
{
	const FVector2d MovementVector = Value.Get<FVector2d>();
	if (APawn* WDNPawn = GetPawn())
	{
		WDNPawn->AddMovementInput(WDNPawn->GetActorForwardVector(), MovementVector.X);
		WDNPawn->AddMovementInput(WDNPawn->GetActorRightVector(), MovementVector.Y);
	}
}

void AWDNPlayerController::Look(const FInputActionValue& Value)
{
	const FVector2d LookAxisVector = Value.Get<FVector2d>();
	AddPitchInput(LookAxisVector.Y);
	AddYawInput(LookAxisVector.X);
}

void AWDNPlayerController::CrouchPressed()
{
	if (WDNCharacter)
	{
		WDNCharacter->Crouch();
	}
}

void AWDNPlayerController::CrouchReleased()
{
	if (WDNCharacter)
	{
		WDNCharacter->UnCrouch();
	}
}

void AWDNPlayerController::WalkPressed()
{
	if (WDNCharacter)
	{
		WDNCharacter->WalkPressed();
	}
}

void AWDNPlayerController::WalkReleased()
{
	if (WDNCharacter)
	{
		WDNCharacter->WalkReleased();
	}
}

void AWDNPlayerController::StartJump()
{
	if (WDNCharacter)
	{
		WDNCharacter->Jump();
	}
}

void AWDNPlayerController::StopJump()
{
	if (WDNCharacter)
	{
		WDNCharacter->StopJumping();
	}
}