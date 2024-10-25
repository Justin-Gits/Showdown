// Fill out your copyright notice in the Description page of Project Settings.


//Custom Classes
#include "GS_TimeArena.h"
#include "GM_TimeArena.h"
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

void AGS_TimeArena::GameStart(float SpawnTime)
{
	if (GetLocalRole() != ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("AGS_TimeArena::GameStart - Insufficient Privilege"));
		return;
	}
	TimeToGenerateSpawnZone = SpawnTime;
	if (TimeToGenerateSpawnZone <= 0.0f)
	{
		TimeToGenerateSpawnZone = 3.0f;				// TODO:  Clamp this value or make sure it can't be less than zero. 
	}
	TeamAlphaSpawnZoneCount = 1;					// TODO:  Counting spawn zones will be refactored to just get the length of the spawn detail array. 
	TeamBravoSpawnZoneCount = 1;
	UE_LOG(LogTemp, Warning, TEXT("AGS_TimeArena - Timers Initialized, Wait: %f seconds"), TimeToGenerateSpawnZone);
	GetWorld()->GetTimerManager().SetTimer(TeamAlphaSpawnZoneHandle, this, &AGS_TimeArena::TeamAlphaSpawnZoneTimer, TimeToGenerateSpawnZone, false);
	GetWorld()->GetTimerManager().SetTimer(TeamBravoSpawnZoneHandle, this, &AGS_TimeArena::TeamBravoSpawnZoneTimer, TimeToGenerateSpawnZone, false);
}

void AGS_TimeArena::TeamAlphaSpawnZoneTimer()
{
	UE_LOG(LogTemp, Warning, TEXT("Team Alpha Has Secured %d Spawn Zones"), TeamAlphaSpawnZoneCount);
	GetWorld()->GetTimerManager().SetTimer(TeamAlphaSpawnZoneHandle, this, &AGS_TimeArena::TeamAlphaSpawnZoneTimer, TimeToGenerateSpawnZone, false);
	TeamAlphaSpawnZoneCount++;
}

void AGS_TimeArena::TeamBravoSpawnZoneTimer()
{
	UE_LOG(LogTemp, Warning, TEXT("Team Bravo Has Secured %d Spawn Zones"), TeamBravoSpawnZoneCount);
	GetWorld()->GetTimerManager().SetTimer(TeamBravoSpawnZoneHandle, this, &AGS_TimeArena::TeamBravoSpawnZoneTimer, TimeToGenerateSpawnZone, false);
	TeamBravoSpawnZoneCount++;
}


