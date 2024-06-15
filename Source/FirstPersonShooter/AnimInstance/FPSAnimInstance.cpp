// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UFPSAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	FPSCharacter = GetCharacterRef();
}

void UFPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	/* Check FPSCharacter */
	if(!IsValid(FPSCharacter))
	{
		FPSCharacter = GetCharacterRef();
	}
	if(!IsValid(FPSCharacter)) return;
	/* Emd Check FPSCharacter */
	// ----------------------------------------------------------------
	
	//	Offsets
	CharacterRotation = GetCharacterRotation(FPSCharacter);
	//	Character Speed
	Speed = GetCharacterSpeed(FPSCharacter);
	// Character booleans
	bIsAccelerating = FPSCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	bIsCrouched = FPSCharacter->bIsCrouched;
	bIsFalling = FPSCharacter->GetCharacterMovement()->IsFalling();
}

ACharacter* UFPSAnimInstance::GetCharacterRef() const
{
	ACharacter* LocalCharacter = Cast<ACharacter>(TryGetPawnOwner());

	if(LocalCharacter && LocalCharacter->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
	{
		ICharacterInterface* CharacterInterface = Cast<ICharacterInterface>(LocalCharacter);
		if(CharacterInterface)
		{
			return CharacterInterface->GetCharacterRef();
		}
	}
	return nullptr;
}

float UFPSAnimInstance::GetCharacterSpeed(const ACharacter* Character)
{
	FVector Velocity = Character->GetVelocity();
	Velocity.Z = 0.f;
	return Velocity.Size();
}

float UFPSAnimInstance::GetCharacterRotation(const ACharacter* Character) const
{
	const FRotator AimRotation = FPSCharacter->GetBaseAimRotation();
	const FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(FPSCharacter->GetVelocity());
	return UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;
}
