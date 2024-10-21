// Fill out your copyright notice in the Description page of Project Settings.


#include "SMC_BaseWeapon.h"
#include "ENUM_TimeArena.h"
#include "PC_Player.h"
#include "CHAR_Player.h"
#include "ACTOR_BaseWeaponProjectile.h"

#pragma region Defaults

USMC_BaseWeapon::USMC_BaseWeapon()
{
	Damage = 10.0f;
}

#pragma endregion

#pragma region Projectile Functions

void USMC_BaseWeapon::FireWeapon()
{
	FString fireWeapon = FString::Printf(TEXT("SMC_BaseWeapon::FireWeapon() - Weapon Component Fired Weapon!"));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, fireWeapon);

	if (BulletProjectileClass != nullptr)
	{
		//Try to fire a projectile
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			//Get Player Controller to reference a projectile spawn point.
			ControllerSelf = Cast<APC_Player>(CharacterSelf->GetController());
			const FRotator BulletProjectileSpawnRotation = ControllerSelf->PlayerCameraManager->GetCameraRotation();
			const FVector BulletProjectileSpawnLocation = GetOwner()->GetActorLocation(); //You can add muzzle offset here if needed. You may need to rotate 


			//Set Spawn Collision Handling Override
			FActorSpawnParameters BulletProjectileSpawnParameters;
			BulletProjectileSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			//Spawn the Bullet Projectile
			World->SpawnActor<AACTOR_BaseWeaponProjectile>(BulletProjectileClass, BulletProjectileSpawnLocation, BulletProjectileSpawnRotation, BulletProjectileSpawnParameters);
		}
	}
}

#pragma endregion
