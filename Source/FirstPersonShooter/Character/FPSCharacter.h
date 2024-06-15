// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "FirstPersonShooter/Interfaces/CharacterInterface.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class FIRSTPERSONSHOOTER_API AFPSCharacter : public ACharacter, public ICharacterInterface
{
	GENERATED_BODY()

public:
	AFPSCharacter();

protected:
	virtual void BeginPlay() override;

	void CheckPlayerControllerAndSetInputMappings();
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void CrouchPressed();
	void CrouchReleased();

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual ACharacter* GetCharacterRef() override;

private:

	/*		Inputs		*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* PlayerMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;


	/*		Spring Arm & Camera */
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	USpringArmComponent* SpringArm;
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	UCameraComponent* Camera;	
};
