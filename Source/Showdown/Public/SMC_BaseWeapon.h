// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "SMC_BaseWeapon.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent))
class SHOWDOWN_API USMC_BaseWeapon : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	//Constructor - Where the base values for the weapon goes
	USMC_BaseWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	float Damage;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void FireWeapon();

};
