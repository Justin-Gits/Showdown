// Fill out your copyright notice in the Description page of Project Settings.


#include "CC_Player.h"


#pragma region Defaults
UCC_Player::UCC_Player()
{
	SetCollisionProfileName("NoCollision");
}

void UCC_Player::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCC_Player, CC_Player_CollisionProfile);

}

void UCC_Player::OnRep_CC_Player_CollisionProfileUpdate()
{
	SetCollisionProfileName(CC_Player_CollisionProfile);
}

#pragma endregion
