// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UFPSAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	//FPSCharacter = GetCharacterRef();
	FPSCharacter = Cast<AFPSCharacter>(TryGetPawnOwner());

}

void UFPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	/* Check FPSCharacter */
	if(!IsValid(FPSCharacter))
	{
		//FPSCharacter = Cast<AFPSCharacter>(TryGetPawnOwner());

		FPSCharacter = GetCharacterRef();
	}
	if(!IsValid(FPSCharacter)) return;
	/* Emd Check FPSCharacter */
	// ----------------------------------------------------------------
	
	//	Offsets
	CharacterRotation = GetCharacterRotation(FPSCharacter);
	AimPitch = GetCharacterAimPitch(FPSCharacter);
	
	//	Character Speed
	Speed = GetCharacterSpeed(FPSCharacter);
	// Character booleans
	bIsAccelerating = FPSCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	bIsCrouched = FPSCharacter->bIsCrouched;
	bIsFalling = FPSCharacter->GetCharacterMovement()->IsFalling();
}

AFPSCharacter* UFPSAnimInstance::GetCharacterRef() const
{
	AFPSCharacter* LocalCharacter = Cast<AFPSCharacter>(TryGetPawnOwner());	
	if(LocalCharacter && LocalCharacter->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
	{
		if(ICharacterInterface* CharacterInterface = Cast<ICharacterInterface>(LocalCharacter))
		{
			return CharacterInterface->GetCharacterRef();
		}
	}
	return nullptr;
}

float UFPSAnimInstance::GetCharacterSpeed(const AFPSCharacter* Character)
{
	FVector Velocity = Character->GetVelocity();
	Velocity.Z = 0.f;
	return Velocity.Size();
}

float UFPSAnimInstance::GetCharacterRotation(const AFPSCharacter* Character) const
{
	const FRotator AimRotation = Character->GetBaseAimRotation();
	const FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(FPSCharacter->GetVelocity());
	return UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;
}

bool UFPSAnimInstance::GetCharacterIsFalling(const AFPSCharacter* Character)
{
	return Character->GetMovementComponent()->IsFalling();
}

float UFPSAnimInstance::GetCharacterAimPitch(const AFPSCharacter* Character)
{
	return Character->AimPitch;
}
