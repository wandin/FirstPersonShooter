// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WDNHUD.generated.h"

class UWDN_TeamSelectionWidget;


/**
 * 
 */
UCLASS()
class WEDEFEATNATIONS_API AWDNHUD : public AHUD
{
	GENERATED_BODY()


public:

	virtual void BeginPlay() override;

	
	/* Team Selection Widget */
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<UUserWidget> TeamSelectionClass;
	UPROPERTY()
	UWDN_TeamSelectionWidget* TeamSelectionWidget;
	void AddTeamSelectionWidget();
	/* End of Team Selection */
	
};