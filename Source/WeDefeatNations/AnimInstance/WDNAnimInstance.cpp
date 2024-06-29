// Fill out your copyright notice in the Description page of Project Settings.


#include "WDNAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UWDNAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	//WDNCharacter = GetCharacterRef();
	WDNCharacter = Cast<AWDNCharacter>(TryGetPawnOwner());
}

void UWDNAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	/* Check WDNCharacter */
	if(!IsValid(WDNCharacter))
	{
		//WDNCharacter = Cast<AWDNCharacter>(TryGetPawnOwner());

		WDNCharacter = GetCharacterRef();
	}
	if(!IsValid(WDNCharacter)) return;
	/* Emd Check WDNCharacter */
	// ----------------------------------------------------------------
	
	//	Offsets
	CharacterRotation = GetCharacterRotation(WDNCharacter);
	AimPitch = GetCharacterAimPitch(WDNCharacter);
	
	//	Character Speed
	Speed = GetCharacterSpeed(WDNCharacter);
	// Character booleans
	bIsAccelerating = WDNCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	bIsCrouched = WDNCharacter->bIsCrouched;
	bIsFalling = WDNCharacter->GetCharacterMovement()->IsFalling();
}

AWDNCharacter* UWDNAnimInstance::GetCharacterRef() const
{
	AWDNCharacter* LocalCharacter = Cast<AWDNCharacter>(TryGetPawnOwner());	
	if(LocalCharacter && LocalCharacter->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
	{
		if(ICharacterInterface* CharacterInterface = Cast<ICharacterInterface>(LocalCharacter))
		{
			return CharacterInterface->GetCharacterRef();
		}
	}
	return nullptr;
}

float UWDNAnimInstance::GetCharacterSpeed(const AWDNCharacter* Character)
{
	FVector Velocity = Character->GetVelocity();
	Velocity.Z = 0.f;
	return Velocity.Size();
}

float UWDNAnimInstance::GetCharacterRotation(const AWDNCharacter* Character) const
{
	const FRotator AimRotation = Character->GetBaseAimRotation();
	const FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(WDNCharacter->GetVelocity());
	return UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;
}

bool UWDNAnimInstance::GetCharacterIsFalling(const AWDNCharacter* Character)
{
	return Character->GetMovementComponent()->IsFalling();
}

float UWDNAnimInstance::GetCharacterAimPitch(const AWDNCharacter* Character)
{
	return Character->AimPitch;
}
