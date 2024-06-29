// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WeDefeatNations/Character/WDNCharacter.h"
#include "WDNAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class WEDEFEATNATIONS_API UWDNAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	AWDNCharacter* GetCharacterRef() const;

private:

	//	Character and properties
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	AWDNCharacter* WDNCharacter;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float CharacterRotation;
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float AimPitch;
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Speed;
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsCrouched;
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsFalling;
	
	static float GetCharacterSpeed(const AWDNCharacter* Character);
	float GetCharacterRotation(const AWDNCharacter* Character) const;
	static bool GetCharacterIsFalling(const AWDNCharacter* Character);

	static float GetCharacterAimPitch(const AWDNCharacter* Character);
};
