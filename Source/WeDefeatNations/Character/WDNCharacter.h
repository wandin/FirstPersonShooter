// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "WeDefeatNations//Interfaces/CharacterInterface.h"
#include "GameFramework/Character.h"
#include "WDNCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class WEDEFEATNATIONS_API AWDNCharacter : public ACharacter, public ICharacterInterface
{
	GENERATED_BODY()

public:
	AWDNCharacter();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaTime) override;
	virtual AWDNCharacter* GetCharacterRef() override;



	void WalkPressed();
	void WalkReleased();
	UFUNCTION(Server, Unreliable)
	void Server_WalkPressed();		// Handles replicating the movement while Press Walking
	UFUNCTION(Server, Unreliable)
	void Server_WalkReleased();		// Handles replicating the movement while Releasing Walking

	/**
	 * We are overriding this function to allow Jumping while Crouching as the default implementation does not allow jump while crouching
	 * @return if we can jump
	 */
	virtual bool CanJumpInternal_Implementation() const override;
	virtual void Jump() override;
	virtual void StopJumping() override;
	
	// We are overriding this function mainly to Play a sound At location when the Character lands
	virtual void Landed(const FHitResult& Hit) override;

	UPROPERTY(Replicated)
	float AimPitch;

protected:
	virtual void BeginPlay() override;
	
	// Default Character Mes (inherited mesh from ACharacter class) - The mesh we see
	UPROPERTY()
	USkeletalMeshComponent* CharacterMesh;
	// A Mesh for casting a proper shadow, as in the (inherited mesh from ACharacter class) we are hiding the Defauolt Mesh's head to attach a camera
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* ShadowMesh;

private:
		
	/*		Spring Arm & Camera */
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	USpringArmComponent* SpringArm;
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	UCameraComponent* Camera;

	// To simplify avoid calling GetCharacterMovement() all the time
	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* JumpLandingSound;
	
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
};
