// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACTOR_BaseWeapon.generated.h"

//Weapon Stats Struct

USTRUCT(BlueprintType)
struct FWeaponStats
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float Damage;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float BulletSpeed;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float FireType;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float FireRate;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float ReloadTime;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float MagazineCapacity;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float AimTime;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float Recoil;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float BulletSpread;
};

UCLASS()
class SHOWDOWN_API AACTOR_BaseWeapon : public AActor
{
	GENERATED_BODY()
#pragma region Default 
public:	
	// Sets default values for this actor's properties
	AACTOR_BaseWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#pragma endregion

};