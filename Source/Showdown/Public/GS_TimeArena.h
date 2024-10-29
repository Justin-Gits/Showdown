// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GS_TimeArena.generated.h"

class ACHAR_Player;
class APC_Player;

UCLASS()
class SHOWDOWN_API AGS_TimeArena : public AGameState
{
	GENERATED_BODY()

public:
	AGS_TimeArena();

protected:
	virtual void Tick(float DeltaTime) override;

#pragma region References
	TArray<APC_Player*> TeamAlphaPlayerControllerList;
	TArray<APC_Player*> TeamBravoPlayerControllerList;

#pragma endregion


#pragma region Spawning
public:
	void GameStart(float SpawnTime);

protected:
	void TeamAlphaSpawnZoneTimer();
	void TeamBravoSpawnZoneTimer();
	int TeamAlphaSpawnZoneCount;
	int TeamBravoSpawnZoneCount;
	TArray<FTransform> TeamAlphaSpawnTransformList;
	TArray<FTransform> TeamBravoSpawnTransformList;

	//Spawn Test Parameters
	bool bSpawnTest = true;


private:
	FTimerHandle TeamAlphaSpawnZoneHandle;
	FTimerHandle TeamBravoSpawnZoneHandle;
	float TimeToGenerateSpawnZone = 10.0f;

#pragma endregion
};
