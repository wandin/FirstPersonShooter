// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FirstPersonShooter/Interfaces/PlayerControllerInterface.h"
#include "GameFramework/PlayerController.h"
#include "FPSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPERSONSHOOTER_API AFPSPlayerController : public APlayerController, public IPlayerControllerInterface
{
	GENERATED_BODY()

public:

	virtual APlayerController* GetPlayerControllerRef() override;
	
};
