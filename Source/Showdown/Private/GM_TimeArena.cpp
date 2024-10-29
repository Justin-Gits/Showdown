// Fill out your copyright notice in the Description page of Project Settings.


//Custom Classes
#include "GM_TimeArena.h"
#include "GS_TimeArena.h"
#include "CHAR_Player.h"
#include "CMC_Player.h"
#include "PC_Player.h"
#include "ENUM_TimeArena.h"
#include "PlayerStart_TimeArena.h"
//Engine Functionality
#include "Engine/Engine.h"


void AGM_TimeArena::SpawnCharacterForPlayer_Implementation(APC_Player* PlayerController)
{
	UE_LOG(LogTemp, Warning, TEXT("AGM_TimeArena::SpawnCharacterForPlayer_Implementation - C++ Native Function Called, Ensure Blueprint Override"));
}

void AGM_TimeArena::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	PlayerControllerList.Add(NewPlayer);
}

void AGM_TimeArena::SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC)
{
	Super::SwapPlayerControllers(OldPC, NewPC);
	PlayerControllerList.Add(NewPC);

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
	if (NumberOfPlayers>1)
	{
		TeamBravoPlayerControllers.Add(PlayerControllerList[1]);
	}
	

	//UE_LOG(LogTemp, Warning, TEXT("Team Alpha Player Controller: %s"), *TeamAlphaPlayerControllers[0]->GetName());
	//UE_LOG(LogTemp, Warning, TEXT("Team Bravo Player Controller: %s"), *TeamBravoPlayerControllers[0]->GetName());
	

}

void AGM_TimeArena::CreateSpawnPoint(APC_Player* PlayerReference)
{
	//Instead of creating spawns, I will be creating a bunch of characters that will be untargettable.  
	
	//Create the Player Start. 
	
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		
		ACHAR_Player* ActiveCharacter = PlayerReference->GetActiveCharacter();
		if (ActiveCharacter == nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("AGM_TimeArena::CreateSpawnPoint - GetActiveCharacter returned nullptr "));
			return;
		}
		FVector SnapshotSpawnLocation = ActiveCharacter->GetActorLocation();
		FRotator SnapshotSpawnRotation = ActiveCharacter->GetActorRotation();
		FVector SnapshotVelocity = ActiveCharacter->GetCMC_Player()->Velocity;
		float SnapshotSpawnHealth = ActiveCharacter->GetCurrentHealth();
		float SnapshotAmmo = 25.0f; // TODO: Change ammo definition here. 

		//Necessary spawn parameters
		FActorSpawnParameters SnapshotSpawnParameters;
		SnapshotSpawnParameters.Owner = PlayerReference;
		SnapshotSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//Spawn the Snapshot

		if (SnapshotSpawnBPClass == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("AGM_TimeArena::CreateSpawnPoint - SnapshotSpawnBPClass == nullptr, make sure it's defined in BP"));
		}
		else
		{
			ACHAR_Player* SnapshotSpawn = World->SpawnActor<ACHAR_Player>(SnapshotSpawnBPClass, SnapshotSpawnLocation, SnapshotSpawnRotation, SnapshotSpawnParameters);
			SnapshotSpawn->InitializeSnapshotSpawn(SnapshotSpawnHealth, SnapshotAmmo, SnapshotVelocity);
		}
		
	}

	//Track the player start in an array. 

}

