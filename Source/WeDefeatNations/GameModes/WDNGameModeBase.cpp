// Fill out your copyright notice in the Description page of Project Settings.


#include "WDNGameModeBase.h"

#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

void AWDNGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	
	if(NewPlayer)
	{
		ConnectedPlayers.AddUnique(NewPlayer);
	}

	TArray<AActor*> T1SpawnPoints;
	TArray<AActor*> T2SpawnPoints;
	
	UGameplayStatics::GetAllActorsOfClass(this, Team1Player, T1SpawnPoints);
	UGameplayStatics::GetAllActorsOfClass(this, Team2Player, T2SpawnPoints);
	
	/*		Print Spawn Points for debuggin */
	for(int32 Index = 0; Index < T1SpawnPoints.Num(); Index++ )
	{
		if(GEngine)
		{
			FString SpawnPointName = T1SpawnPoints[Index]->GetName();
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("T1 Spawn Point: %s"), *SpawnPointName));
		}
	}

	for(int32 Index = 0; Index < T2SpawnPoints.Num(); Index++ )
	{
		if(GEngine)
		{
			FString SpawnPointName = T2SpawnPoints[Index]->GetName();
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("T2 Spawn Point: %s"), *SpawnPointName));
		}
	}
	/*----------------------------------*/

	
}
