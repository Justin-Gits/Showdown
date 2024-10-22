// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "SMC_BaseWeapon.generated.h"

class AACTOR_BaseWeaponProjectile; 
class ACHAR_Player;
class APC_Player;


UCLASS(Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent))
class SHOWDOWN_API USMC_BaseWeapon : public USkeletalMeshComponent
{
	GENERATED_BODY()


#pragma region Defaults
protected:
	//Constructor - Where the base values for the weapon goes
	USMC_BaseWeapon();
#pragma endregion


#pragma region Projectile Functions
public:
	//Projectile class to spawn
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<AACTOR_BaseWeaponProjectile> BulletProjectileClass;

	//Checks authority on player requesting to fire the weapon
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void NetworkRequestFireWeapon(ACHAR_Player* RequestingCharacter);

	//Logic to fire the weapon
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void FireWeapon(ACHAR_Player* RequestingCharacter);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FVector MuzzleOffset;
#pragma endregion


#pragma region Referenced Objects
protected:
	ACHAR_Player* CharacterSelf;
	APC_Player* ControllerSelf;

#pragma endregion


#pragma region RPCs
protected:
	UFUNCTION(Server, Reliable)
	void ServerSpawnBulletProjectile(ACHAR_Player* RequestingCharacter);


#pragma endregion
};
