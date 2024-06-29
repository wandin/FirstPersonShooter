// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WDNGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class WEDEFEATNATIONS_API AWDNGameModeBase : public AGameModeBase
{
	GENERATED_BODY()


public:


protected:
	
	void OnPostLogin(AController* NewPlayer) override;

	UPROPERTY()
	TArray<AController*> ConnectedPlayers;


	UPROPERTY(EditAnywhere)
	TSubclassOf<APlayerStart> Team1Player;
	UPROPERTY(EditAnywhere)
	TSubclassOf<APlayerStart> Team2Player;

};
