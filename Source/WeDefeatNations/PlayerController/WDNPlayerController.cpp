// Fill out your copyright notice in the Description page of Project Settings.


#include "WDNPlayerController.h"

void AWDNPlayerController::BeginPlay()
{
	Super::BeginPlay();

	HandleWidget(EGameWidgets::EGW_TeamSelection, true);
}

APlayerController* AWDNPlayerController::GetPlayerControllerRef()
{
	return this;
}

void AWDNPlayerController::HandleWidget(EGameWidgets Widget, bool ShoudlDisplayUI)
{
	if(ShoudlDisplayUI)
	{
		DisplayWidget(Widget);
	}
	HideWidget(Widget);
}

void AWDNPlayerController::DisplayWidget(EGameWidgets WidgetToDisplay)
{
}

void AWDNPlayerController::HideWidget(EGameWidgets WidgetToHide)
{
}
