// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTOR_BaseWeapon.h"

// Sets default values
AACTOR_BaseWeapon::AACTOR_BaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AACTOR_BaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AACTOR_BaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


