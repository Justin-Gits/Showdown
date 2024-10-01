// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_TimeArena.h"
#include "PC_Player.h"


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
