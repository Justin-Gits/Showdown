// Fill out your copyright notice in the Description page of Project Settings.


#include "CHAR_Player.h"

// Sets default values
ACHAR_Player::ACHAR_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Network Replication
	bReplicates = true;
	SetReplicateMovement(true);

}

void ACHAR_Player::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

// Called when the game starts or when spawned
void ACHAR_Player::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACHAR_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACHAR_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

