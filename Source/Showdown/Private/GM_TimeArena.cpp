// Fill out your copyright notice in the Description page of Project Settings.


//Custom Classes
#include "GM_TimeArena.h"
#include "GS_TimeArena.h"
#include "PC_Player.h"
#include "ENUM_TimeArena.h"
#include "CHAR_Player.h"
#include "CMC_Player.h"
//Engine Functionality
#include "Engine/Engine.h"
#include "Engine/World.h"


void AGM_TimeArena::SpawnCharacterForPlayer_Implementation(APC_Player* PlayerController)
{
	UE_LOG(LogTemp, Warning, TEXT("AGM_TimeArena::SpawnCharacterForPlayer_Implementation - C++ Native Function Called, Ensure Blueprint Override"));
}

void AGM_TimeArena::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	APC_Player* CastNewPlayer = Cast<APC_Player>(NewPlayer);
	PlayerControllerList.Add(CastNewPlayer);
}

void AGM_TimeArena::SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC)
{
	Super::SwapPlayerControllers(OldPC, NewPC);
	APC_Player* CastNewPC = Cast<APC_Player>(NewPC);
	PlayerControllerList.Add(CastNewPC);

}

void AGM_TimeArena::Logout(AController* ExitingController)
{
	APC_Player* ExitingPlayer = Cast<APC_Player>(ExitingController);
	PlayerControllerList.Remove(ExitingPlayer);
}

void AGM_TimeArena::StartGame()
{
	FString StartGame = FString::Printf(TEXT("Let the games, Begin!"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, StartGame);
	GameStateTimeArena = GetGameState<AGS_TimeArena>();
	AssignTeam();
	if (GameStateTimeArena)
	{
		GameStateTimeArena->GameStart(SpawnZoneTimeInterval);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AGM_TimeArena::StartGame() - GameStateTimeArena = nullptr"));
	}
}

void AGM_TimeArena::AssignTeam()
{
	// TODO: The team assignment is crudely hard coded in, later I should make this modular for multiple players. 
	int32 NumberOfPlayers = PlayerControllerList.Num();
	UE_LOG(LogTemp, Warning, TEXT("NumberOfPlayers = %d"), NumberOfPlayers);

	TeamAlphaPlayerControllers.Add(PlayerControllerList[0]);
	TeamBravoPlayerControllers.Add(PlayerControllerList[1]);

	//UE_LOG(LogTemp, Warning, TEXT("Team Alpha Player Controller: %s"), *TeamAlphaPlayerControllers[0]->GetName());
	//UE_LOG(LogTemp, Warning, TEXT("Team Bravo Player Controller: %s"), *TeamBravoPlayerControllers[0]->GetName());
	

}
void AGM_TimeArena::RequestSnapshotSpawn(APC_Player* PlayerReference)
{
	CreateSnapshotSpawnPoint(PlayerReference);
}
//Create a snapshot spawn point based on the player's current position
void AGM_TimeArena::CreateSnapshotSpawnPoint(APC_Player* PlayerReference)
{
	UWorld* const World_Reference = GetWorld();
	if (World_Reference != nullptr)
	{
		ACHAR_Player* CHAR_Reference = PlayerReference->GetActiveCharacter();
		if (CHAR_Reference == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("AGM_TimeArena::CreateSpawnPoint - GetActiveCharacter returned nullptr"));
			return;
		}
		
		//Snapshot Information
		FVector SnapshotSpawnLocation = CHAR_Reference->GetActorLocation();
		FRotator SnapshotSpawnRotation = CHAR_Reference->GetActorRotation();
		FVector SnapshotSpawnVelocity = CHAR_Reference->GetCMC_Player()->Velocity;
		float SnapshotSpawnHealth = CHAR_Reference->GetCurrentHealth();
		float SnapshotSpawnAmmo = 25.0f; // TODO: Update this when I incorporate ammunition

		//Spawn Parameters
		FActorSpawnParameters SnapshotSpawnParameters;
		SnapshotSpawnParameters.Owner = PlayerReference;
		SnapshotSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//Spawn the Snapshot
		ensureMsgf(SnapshotSpawnBPClass != nullptr, TEXT("SnapshotSpawnBPClass is not initialized in UE Editor"));
		if (SnapshotSpawnBPClass == nullptr) { return; }
		ACHAR_Player* SnapshotSpawn = World_Reference->SpawnActor<ACHAR_Player>(SnapshotSpawnBPClass, SnapshotSpawnLocation, SnapshotSpawnRotation, SnapshotSpawnParameters);

		// TODO:  Initialize the rest of the snapshot spawn parameters: Health, Ammo, Velocity.  See checkin v.20 from main branch for info.

		// TODO:  I need to start tracking the player spawns in some sort of array. 


	}
}
