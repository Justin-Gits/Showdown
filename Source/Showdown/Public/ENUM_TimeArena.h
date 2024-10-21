// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ENUM_TimeArena.generated.h"


#pragma region Weapons
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_None		UMETA(DisplayName = "None"),
	EWT_Blaster		UMETA(DisplayName = "Blaster")
};

UENUM(BlueprintType)
enum class EFireType : uint8
{
	EFT_Semi		UMETA(DisplayName = "Semi-Auto"),
	EFT_Full		UMETA(DisplayName = "Full-Auto")
};




#pragma endregion