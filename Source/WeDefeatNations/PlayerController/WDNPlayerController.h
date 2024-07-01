// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeDefeatNations/Enums/GameWidgets.h"
#include "WeDefeatNations/Interfaces/PlayerControllerInterface.h"
#include "GameFramework/PlayerController.h"
#include "WeDefeatNations/Character/WDNCharacter.h"
#include "WDNPlayerController.generated.h"

struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
class AWDNHUD;

/**
 * 
 */
UCLASS()
class WEDEFEATNATIONS_API AWDNPlayerController : public APlayerController, public IPlayerControllerInterface, public ICharacterInterface
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	virtual APlayerController* GetPlayerControllerRef() override;
	virtual void SetupInputComponent() override;

	AWDNCharacter* GetCharacterRef() override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* PlayerMappingContext;
	
	/*		Inputs		*/
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
	
	UPROPERTY()
	AWDNHUD* GameHUD;
	UPROPERTY()
	AWDNCharacter* WDNCharacter;

protected:

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void CrouchPressed();			// handles Crouch input Pressed
	void CrouchReleased();			// handles Crouch input Released
	void WalkPressed();				// handles Walk input Pressed
	void WalkReleased();			// handles Walk input Released
	void StartJump();
	void StopJump();
};
