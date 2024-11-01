// Fill out your copyright notice in the Description page of Project Settings.


//Custom Classes
#include "GS_TimeArena.h"
#include "GM_TimeArena.h"
#include "PC_Player.h"
//Engine Functionality
#include "TimerManager.h"
#include "Engine/World.h"


AGS_TimeArena::AGS_TimeArena()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGS_TimeArena::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}




#pragma region Spawning

void AGS_TimeArena::GameStart(float SpawnTime, TArray<APC_Player*> AlphaControllers, TArray<APC_Player*> BravoControllers)
{
	if (GetLocalRole() != ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("AGS_TimeArena::GameStart - Insufficient Privilege"));
		return;
	}
	TeamAlphaPlayerControllerList = AlphaControllers;
	TeamBravoPlayerControllerList = BravoControllers;

	TimeToGenerateSpawnZone = SpawnTime;
	if (TimeToGenerateSpawnZone <= 0.0f)
	{
		TimeToGenerateSpawnZone = 10.0f;				// TODO:  Clamp this value or make sure it can't be less than zero. 
	}
	TeamAlphaSpawnZoneCount = 1;					// TODO:  I should place an initial spawn at the locations where the players start.  
	TeamBravoSpawnZoneCount = 1;
	UE_LOG(LogTemp, Warning, TEXT("AGS_TimeArena - Timers Initialized, Wait: %f seconds"), TimeToGenerateSpawnZone);
	GetWorld()->GetTimerManager().SetTimer(TeamAlphaSpawnZoneHandle, this, &AGS_TimeArena::TeamAlphaSpawnZoneTimer, TimeToGenerateSpawnZone, false);
	GetWorld()->GetTimerManager().SetTimer(TeamBravoSpawnZoneHandle, this, &AGS_TimeArena::TeamBravoSpawnZoneTimer, TimeToGenerateSpawnZone, false);
}

void AGS_TimeArena::TeamAlphaSpawnZoneTimer()
{
	UE_LOG(LogTemp, Warning, TEXT("Team Alpha Has Secured %d Spawn Zones"), TeamAlphaSpawnZoneCount);
	// Create our new spawn point or create information about the spawn point
	// If we're creating a spawnpoint object we will:
	// Spawn the spawnpoint object at the player position (this will cause the server to create the object and send it to the clients)
	// Change the player data in the spawn point that tracks information such as health (position will be tracked by the object itself0
	// Add a reference to the new spawnpoint in an array
	// Assuming the spawnpoints are replicated a lot of this should "just work"

	// If we're just tracking the information and handling the respawning in this class:
	// We'll create an instance of a struct (or class) and put in the position information and other player info (health) into it
	// Add a reference to the new struct in an array
	// You may or may not run into some trickyness with syncronizing data or making sure the data is all on the server

	TeamAlphaSpawnZoneCount++;

	if (bSpawnTest == true)
	{
		AGM_TimeArena* CurrentGameMode = Cast<AGM_TimeArena>(GetWorld()->GetAuthGameMode());
		ensureMsgf(CurrentGameMode != nullptr, TEXT("CurrentGameMode = nullptr"));
		APC_Player* TeamAlphaPlayer = TeamAlphaPlayerControllerList[0];
		ensureMsgf(CurrentGameMode != nullptr, TEXT("TeamAlphaPlayerControllerList [0] = nullptr"));
		if (HasAuthority())
		{
			CurrentGameMode->CreateSpawnPoint(TeamAlphaPlayer);
		}

		//Set up logic to spawn a spawnpoint
	}
	GetWorld()->GetTimerManager().SetTimer(TeamAlphaSpawnZoneHandle, this, &AGS_TimeArena::TeamAlphaSpawnZoneTimer, TimeToGenerateSpawnZone, false);
}

void AGS_TimeArena::TeamBravoSpawnZoneTimer()
{
	UE_LOG(LogTemp, Warning, TEXT("Team Bravo Has Secured %d Spawn Zones"), TeamBravoSpawnZoneCount);
	if (bSpawnTest == true)
	{
		AGM_TimeArena* CurrentGameMode = Cast<AGM_TimeArena>(GetWorld()->GetAuthGameMode());
		ensureMsgf(CurrentGameMode != nullptr, TEXT("CurrentGameMode = nullptr"));
		APC_Player* TeamBravoPlayer = TeamBravoPlayerControllerList[0];
		ensureMsgf(CurrentGameMode != nullptr, TEXT("TeamBravoPlayerControllerList [0] = nullptr"));
		if (HasAuthority())
		{
			CurrentGameMode->CreateSpawnPoint(TeamBravoPlayer);
		}

		bSpawnTest = false;
	}
	
	
	GetWorld()->GetTimerManager().SetTimer(TeamBravoSpawnZoneHandle, this, &AGS_TimeArena::TeamBravoSpawnZoneTimer, TimeToGenerateSpawnZone, false);

	TeamBravoSpawnZoneCount++;
}

#pragma endregion
