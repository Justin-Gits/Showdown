// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "PlayerStart_TimeArena.generated.h"

/**
 * 
 */
UCLASS()
class SHOWDOWN_API APlayerStart_TimeArena : public APlayerStart
{
	GENERATED_BODY()

	friend class GM_TimeArena;


#pragma region Spawn Information
protected:
	UPROPERTY()
	FVector SpawnLocation;
	UPROPERTY()
	FRotator SpawnRotation;
	UPROPERTY()
	float SpawnHealth;
	UPROPERTY()
	float SpawnAmmo;
	//Spawn Weapon
	//Spawn Speed?
	//Spawn stance (Standing, crouching, etc)

#pragma endregion
};
