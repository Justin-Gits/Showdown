// Fill out your copyright notice in the Description page of Project Settings.


//Custom Classes
#include "PS_Player.h"
#include "CHAR_Player.h"
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
		CreateSnapshotSpawn();
	}
	
}

void APS_Player::CreateSnapshotSpawn()
{
	if (HasAuthority() && RespawnAllowed == true)
	{
		PC_Reference = Cast<APC_Player>(GetPlayerController());
		UE_LOG(LogTemp, Warning, TEXT("APS_Player::CreateSnapshotSpawn() - Created Snapshot Spawn"));
		GM_Reference->RequestSnapshotSpawn(PC_Reference);

		if (GMEnableSnapshotSpawns == true)
		{
			GetWorld()->GetTimerManager().SetTimer(SnapshotSpawnTimerHandle, this, &APS_Player::CreateSnapshotSpawn, PSSpawnZoneTimeInterval, false);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("APS_Player::CreateSnapshotSpawn() - Further Snapshot Spawns Disabled, Check EnableSnapshotSpawns parameter in UE UI"));
		}
	}
	
}

void APS_Player::RequestAddToSnapshotArray(ACHAR_Player* NewSnapshot)
{
	if (HasAuthority())
	{
		AddToSnapshotArray(NewSnapshot);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("APS_Player::RequestAddToSnapshotArray - Insufficient Priveleges"));
	}
}

void APS_Player::DebugSpawnArrayPrintout()
{
	UE_LOG(LogTemp, Warning, TEXT("SpawnableCharacters[%d]"), SnapshotCharacterArray.Num());
}

ACHAR_Player* APS_Player::GetSnapshotSpawn()
{
	if (HasAuthority())
	{
		//Get snapshot information
		if (SnapshotCharacterArray.Num() <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("APS_Player::GetSnapshotSpawn - No Spawns Available, You Lose :("));
			RespawnAllowed = false;
			StopSnapshotTimer();
			ClientStopSnapshotTimer();
			return nullptr;
		}
		else
		{
			LatestSnapshot = SnapshotCharacterArray.Last();
			SnapshotCharacterArray.Pop();
			return LatestSnapshot;
		}
	}
	else
	{
		if (SnapshotCharacterArray.Num() <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("APS_Player::GetSnapshotSpawn - No Spawns Available, You Lose :("));
			StopSnapshotTimer();
			ServerStopSnapshotTimer();
			return nullptr;
		}
		else
		{
			LatestSnapshot = SnapshotCharacterArray.Last();
			ServerGetSnapshotSpawn(LatestSnapshot);
			return LatestSnapshot;
		}
	}
}

void APS_Player::RequestRestartSnapshotTimer()
{
	ExecuteRestartSnapshotTimer();
}

void APS_Player::RequestStopSnapshotTimer()
{
	StopSnapshotTimer();
}

void APS_Player::AddToSnapshotArray(ACHAR_Player* NewSnapshot)
{
	if (HasAuthority())
	{
		SnapshotCharacterArray.Add(NewSnapshot);
		UE_LOG(LogTemp, Warning, TEXT("APS_Player::AddToSnapshotArray - Added Snapshot: %s"), *NewSnapshot->GetName());
		//Debug, name out snapshots
		int PrintingLength = SnapshotCharacterArray.Num();
		for (int i = 0; i < PrintingLength; i++)
		{
			FString SnapshotName = NewSnapshot->GetName();
			UE_LOG(LogTemp, Warning, TEXT("APS_Player:: AddToSnapshotArray: entry %d = %s"), i, *SnapshotName);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("APS_Player::AddToSnapshotArray - Lacked Authority to add snapshot"));
	}
}

const void APS_Player::SetGMSpawnZoneTimeInterval()
{
	GM_Reference = Cast<AGM_TimeArena>(GetWorld()->GetAuthGameMode());
	GMSpawnZoneTimeInterval = GM_Reference->GetSpawnZoneTimeInterval();
	GMEnableSnapshotSpawns = GM_Reference->GetEnableSnapshotSpawns();
}



//If a character perishes before creating a new snapshot, restart the "create a snapshot" timer. 
void APS_Player::ExecuteRestartSnapshotTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(SnapshotSpawnTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(SnapshotSpawnTimerHandle, this, &APS_Player::CreateSnapshotSpawn, PSSpawnZoneTimeInterval, false);
}

void APS_Player::StopSnapshotTimer()
{
	if (GetWorldTimerManager().IsTimerActive(SnapshotSpawnTimerHandle))
	{
		UE_LOG(LogTemp, Warning, TEXT("APS_Player::StopSnapshotTimer() - Timer Handle was active, now stopping."));
		GetWorld()->GetTimerManager().ClearTimer(SnapshotSpawnTimerHandle);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("APS_Player::StopSnapshotTimer() - Timer Handle was not active, failing silently."));
	}

}

void APS_Player::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APS_Player, RespawnAllowed);
}

void APS_Player::ServerStopSnapshotTimer_Implementation()
{
	RespawnAllowed = false;
	StopSnapshotTimer();
}

void APS_Player::ClientStopSnapshotTimer_Implementation()
{
	StopSnapshotTimer();
}

void APS_Player::ServerGetSnapshotSpawn_Implementation(ACHAR_Player* TargetSnapshot)
{
	SnapshotCharacterArray.Pop();
	
}

bool APS_Player::ServerGetSnapshotSpawn_Validate(ACHAR_Player* TargetSnapshot)
{
	if (SnapshotCharacterArray.Last() == TargetSnapshot)
	{
		//Client's latest snapshot matches server snapshot
		UE_LOG(LogTemp, Warning, TEXT("APS_Player::ServerGetSnapshotSpawn_Validate - Client and Server match"));
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("APS_Player::ServerGetSnapshotSpawn_Validate - Client and Server MISMATCH."));
		return false;
	}
}
void APS_Player::ClientGetSnapshotSpawn_Implementation(const TArray<ACHAR_Player*>& ServerCharacterArray)
{
	if (ServerCharacterArray != SnapshotCharacterArray)
	{
		UE_LOG(LogTemp, Warning, TEXT("APS_Player::ClientGetSnapshotSpawn - Server and Client Snapshot Arrays do not match, fixing."));
		SnapshotCharacterArray = ServerCharacterArray;
	}
}
#pragma endregion
