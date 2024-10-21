// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACTOR_BaseWeapon.generated.h"

//Weapon Stats Struct



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



#pragma region Setter Getter Functions

#pragma endregion
};