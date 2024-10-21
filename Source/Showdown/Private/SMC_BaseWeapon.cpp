// Fill out your copyright notice in the Description page of Project Settings.


#include "SMC_BaseWeapon.h"
#include "ENUM_TimeArena.h"


USMC_BaseWeapon::USMC_BaseWeapon()
{
	Damage = 10.0f;
}

void USMC_BaseWeapon::FireWeapon()
{
	FString fireWeapon = FString::Printf(TEXT("SMC_BaseWeapon::FireWeapon() - Weapon Component Fired Weapon!"));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, fireWeapon);
}
