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
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual AFPSCharacter* GetCharacterRef() override;

protected:
	virtual void BeginPlay() override;

	void CheckPlayerControllerAndSetInputMappings() const;
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void CrouchPressed();
	void CrouchReleased();
	void WalkPressed();
	void WalkReleased();

	virtual void Jump() override;
	virtual void StopJumping() override;


	UFUNCTION(Server, Unreliable)
	void Server_WalkPressed();
	UFUNCTION(Server, Unreliable)
	void Server_WalkReleased();


	// A Mesh for casting a proper shadow, as in the (inherited mesh from ACharacter class) we are hiding the head to attach a camera
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* ShadowMesh;

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* WalkAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	
	/*		Spring Arm & Camera */
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	USpringArmComponent* SpringArm;
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	UCameraComponent* Camera;
	
	//	Functions
	UFUNCTION()
	void CalculateAimDirection();

	UFUNCTION(Server, Reliable)
	void Server_CalculateAimDirection();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateAimPitch(float NewAimPitch);

	FORCEINLINE float GetAimDirection() const { return AimPitch; }

	UPROPERTY(Replicated)
	bool bWalking;

public:	

	UPROPERTY(Replicated)
	float AimPitch;
	
};
