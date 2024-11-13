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
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}

#pragma endregion

#pragma region Projectile Functions

void USMC_BaseWeapon::NetworkRequestFireWeapon(ACHAR_Player* RequestingCharacter)
{
	if (GetOwner()->HasAuthority())
	{
		FireWeapon(RequestingCharacter);
	}
	else
	{
		ServerSpawnBulletProjectile(RequestingCharacter);
		FireWeapon(RequestingCharacter);
	}
	

}
void USMC_BaseWeapon::FireWeapon(ACHAR_Player* RequestingCharacter)
{
	//FString fireWeapon = FString::Printf(TEXT("SMC_BaseWeapon::FireWeapon() - Weapon Component Fired Weapon!"));
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, fireWeapon);

	if (RequestingCharacter == nullptr)
	{
		FString invalidCharRef = FString::Printf(TEXT("SMC_BaseWeapon::FireWeapon() - invalid Character Reference"));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, invalidCharRef);
		return;
	}

	else if (BulletProjectileClass != nullptr)
	{
		//Try to fire a projectile
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			//Get Player Controller to reference a projectile spawn point.
			CharacterSelf = RequestingCharacter;
			ControllerSelf = Cast<APC_Player>(CharacterSelf->GetController());
			const FRotator BulletProjectileSpawnRotation = ControllerSelf->PlayerCameraManager->GetCameraRotation();
			const FVector BulletProjectileSpawnLocation = CharacterSelf->GetActorLocation() + BulletProjectileSpawnRotation.RotateVector(MuzzleOffset);
			FVector TestProjectileSpawnLocation = FVector(100.0f, 100.0f, 100.0f);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters BulletProjectileSpawnParameters;
			BulletProjectileSpawnParameters.Owner = GetOwner();
			//BulletProjectileSpawnParameters.Instigator = GetInstigator();
			BulletProjectileSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			//Spawn the Bullet Projectile
			AACTOR_BaseWeaponProjectile* BulletProjectile=World->SpawnActor<AACTOR_BaseWeaponProjectile>(BulletProjectileClass, BulletProjectileSpawnLocation, BulletProjectileSpawnRotation, BulletProjectileSpawnParameters);
			BulletProjectile->SetInstigator(CharacterSelf);


		}
	}

}
#pragma endregion

#pragma region RPCs
void USMC_BaseWeapon::ServerSpawnBulletProjectile_Implementation(ACHAR_Player* RequestingCharacter)
{
	//FString fireWeapon = FString::Printf(TEXT("USMC_BaseWeapon::ServerSpawnBulletProjectile_Implementation() - Spawning a Bullet on the server"));
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, fireWeapon);
	FireWeapon(RequestingCharacter);
}


#pragma endregion

