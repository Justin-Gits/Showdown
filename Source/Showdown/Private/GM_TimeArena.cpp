// Fill out your copyright notice in the Description page of Project Settings.


//Custom Classes
#include "GM_TimeArena.h"
#include "GS_TimeArena.h"
#include "PC_Player.h"
#include "ENUM_TimeArena.h"
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
	TeamBravoPlayerControllers.Add(PlayerControllerList[1]);

	//UE_LOG(LogTemp, Warning, TEXT("Team Alpha Player Controller: %s"), *TeamAlphaPlayerControllers[0]->GetName());
	//UE_LOG(LogTemp, Warning, TEXT("Team Bravo Player Controller: %s"), *TeamBravoPlayerControllers[0]->GetName());
	

}
