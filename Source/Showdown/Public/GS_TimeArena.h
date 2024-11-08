// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GS_TimeArena.generated.h"

class AGM_TimeArena;

UCLASS()
class SHOWDOWN_API AGS_TimeArena : public AGameState
{
	GENERATED_BODY()

public:
	AGS_TimeArena();

protected:
	virtual void Tick(float DeltaTime) override;

#pragma region Spawning Zone Tracking Systems
public:
	void GameStart(float SpawnTime);
	
	UPROPERTY(EditDefaultsOnly, Category="Snapshot Spawn")
	float TimeToGenerateSpawnZone = 3.0f;

protected:
	void TeamAlphaSpawnZoneTimer();
	void TeamBravoSpawnZoneTimer();
	int TeamAlphaSpawnZoneCount;
	int TeamBravoSpawnZoneCount;
	FTimerHandle TeamAlphaSpawnZoneHandle;
	FTimerHandle TeamBravoSpawnZoneHandle;

#pragma endregion
};
