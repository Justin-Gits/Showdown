// Fill out your copyright notice in the Description page of Project Settings.


//Custom Classes
#include "PS_Player.h"
#include "PC_Player.h"
#include "GM_TimeArena.h"
//Engine Functionality
#include "TimerManager.h"
#include "Engine/World.h"


APS_Player::APS_Player()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APS_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#pragma region Snapshot Spawn
void APS_Player::BeginSpawnTimers()
{
	if (HasAuthority())
	{
		if (GMSpawnZoneTimeInterval <= 0.1f)
		{
			UE_LOG(LogTemp, Warning, TEXT("APS_Player::BeginSpawnTimers - Spawn timer <= 0.1 seconds, setting to default 3 seconds "));
			PSSpawnZoneTimeInterval = 3.0f;
		}
		else
		{
			PSSpawnZoneTimeInterval = GMSpawnZoneTimeInterval;
		}
		GetWorld()->GetTimerManager().SetTimer(SnapshotSpawnTimerHandle, this, &APS_Player::CreateSnapshotSpawn, PSSpawnZoneTimeInterval, false);
	}
	
}

const void APS_Player::SetGMSpawnZoneTimeInterval()
{
	GM_Reference = Cast<AGM_TimeArena>(GetWorld()->GetAuthGameMode());
	GMSpawnZoneTimeInterval = GM_Reference->GetSpawnZoneTimeInterval();
	GMEnableSnapshotSpawns = GM_Reference->GetEnableSnapshotSpawns();
}

void APS_Player::CreateSnapshotSpawn()
{
	PC_Reference = Cast<APC_Player>(GetPlayerController());
	UE_LOG(LogTemp, Warning, TEXT("APS_Player::CreateSnapshotSpawn() - Created Snapshot Spawn"));
	if (HasAuthority())
	{
		GM_Reference->RequestSnapshotSpawn(PC_Reference);
	}
	if (GMEnableSnapshotSpawns == true)
	{
		GetWorld()->GetTimerManager().SetTimer(SnapshotSpawnTimerHandle, this, &APS_Player::CreateSnapshotSpawn, PSSpawnZoneTimeInterval, false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("APS_Player::CreateSnapshotSpawn() - Further Snapshot Spawns Disabled, Check EnableSnapshotSpawns parameter in UE UI"));
	}
}
#pragma endregion
