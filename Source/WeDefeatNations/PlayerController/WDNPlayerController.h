// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeDefeatNations/Enums/GameWidgets.h"
#include "WeDefeatNations/Interfaces/PlayerControllerInterface.h"
#include "GameFramework/PlayerController.h"
#include "WDNPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class WEDEFEATNATIONS_API AWDNPlayerController : public APlayerController, public IPlayerControllerInterface
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	virtual APlayerController* GetPlayerControllerRef() override;


	static void HandleWidget(EGameWidgets Widget, bool ShouldDisplayUI);

	static void DisplayWidget(EGameWidgets WidgetToDisplay);
	static void HideWidget(EGameWidgets WidgetToHide);
	
};
