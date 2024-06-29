// Fill out your copyright notice in the Description page of Project Settings.


#include "WDNHUD.h"

#include "WDN_TeamSelectionWidget.h"
#include "Blueprint/UserWidget.h"


void AWDNHUD::BeginPlay()
{
	Super::BeginPlay();
}

void AWDNHUD::AddTeamSelectionWidget()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if(PlayerController && TeamSelectionClass)
	{
		TeamSelectionWidget = CreateWidget<UWDN_TeamSelectionWidget>(PlayerController, TeamSelectionClass);
		TeamSelectionWidget->AddToViewport();
	}
}
