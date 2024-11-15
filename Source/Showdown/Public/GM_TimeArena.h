// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GM_TimeArena.generated.h"

class AGS_TimeArena;
class ACHAR_Player;
class APS_Player;

UCLASS()
class SHOWDOWN_API AGM_TimeArena : public AGameMode
{
	GENERATED_BODY()
	
public:

UFUNCTION(BlueprintNativeEvent, Category="Server")
void SpawnCharacterForPlayer(APC_Player* PlayerController);

void SpawnCharacterForPlayer_Implementation(APC_Player* PlayerController);

protected:


UFUNCTION()
virtual void PostLogin(APlayerController* NewPlayer) override;

UFUNCTION()
virtual void SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC) override;

UFUNCTION()
virtual void Logout(AController* ExitingController) override;

UFUNCTION(Exec)
void StartGame();


UFUNCTION(BlueprintCallable, Category = "Game Setup")
void AssignTeam();

#pragma region References
public:
	const TArray<APC_Player*> GetPlayerControllerList() { return PlayerControllerList;}
	const float GetSpawnZoneTimeInterval() { return SpawnZoneTimeInterval; }

protected:
	TArray<APC_Player*> PlayerControllerList;
	AGS_TimeArena* GameStateTimeArena;


#pragma endregion

#pragma region Teams
protected:
	UPROPERTY()
	TArray<APC_Player*> TeamAlphaPlayerControllers;
UPROPERTY()
	TArray<APC_Player*> TeamBravoPlayerControllers;
#pragma endregion

#pragma region Character Possession
public: 
	UFUNCTION()
	void PossessCharacterUsingPlayer(ACHAR_Player* TargetCharacter, APC_Player* TargetController);

#pragma endregion


#pragma region Snapshot Spawn
public:
	UFUNCTION()
	void RequestSnapshotSpawn(APC_Player* PlayerReference);

	UFUNCTION()
	bool GetEnableSnapshotSpawns() { return EnableSnapshotSpawns; }

protected:
	UFUNCTION()
	void CreateSnapshotSpawnPoint(APC_Player* PlayerReference);

	UPROPERTY(EditDefaultsOnly, Category = "Custom Game Properties")
	TSubclassOf<ACHAR_Player> SnapshotSpawnBPClass;

	UPROPERTY(EditDefaultsOnly, Category = "Custom Game Properties")
	bool EnableSnapshotSpawns;

#pragma endregion

#pragma region Custom Game Properties
protected:

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Custom Game Properties")
	float SpawnZoneTimeInterval;

#pragma endregion


};
