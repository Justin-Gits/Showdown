// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GS_TimeArena.h"
#include "PS_Player.generated.h"

class ACHAR_Player;
class AGS_TimeArena;
class AGM_TimerArena;
class APC_Player;

UCLASS()
class SHOWDOWN_API APS_Player : public APlayerState
{
	GENERATED_BODY()
public:
	APS_Player();

protected:
	virtual void Tick(float DeltaTime) override;


#pragma region Snapshot Spawn
public:
	UFUNCTION()
	const void SetGMSpawnZoneTimeInterval();
	
	UFUNCTION()
	void BeginSpawnTimers();

	UFUNCTION()
	void RequestAddToSnapshotArray(ACHAR_Player* NewSnapshot);

	UFUNCTION()
	void RequestSpawnArrayPrintout();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Custom Properties")
	TArray<ACHAR_Player*> SnapshotCharacterArray;
	
	UFUNCTION()
	void AddToSnapshotArray(ACHAR_Player* NewSnapshot);

	AGS_TimeArena* GS_Reference;
	AGM_TimeArena* GM_Reference;
	APC_Player* PC_Reference;
	float GMSpawnZoneTimeInterval;
	bool GMEnableSnapshotSpawns;
	float PSSpawnZoneTimeInterval;
	int SpawnZoneCount;
	

	
	UFUNCTION()
	void CreateSnapshotSpawn();

private:
	FTimerHandle SnapshotSpawnTimerHandle;

#pragma endregion

};
