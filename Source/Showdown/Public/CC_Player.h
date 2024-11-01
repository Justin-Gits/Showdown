// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "CC_Player.generated.h"


/**
 * 
 */
UCLASS()
class SHOWDOWN_API UCC_Player : public UCapsuleComponent
{
	GENERATED_BODY()
	
#pragma region Defaults
public:
	UCC_Player();

protected:
	//Add custom properties to be replicated
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#pragma endregion

#pragma region Collision
public:
	UPROPERTY(ReplicatedUsing = OnRep_CC_Player_CollisionProfileUpdate)
	FName CC_Player_CollisionProfile;

protected:	

	UFUNCTION()
	void OnRep_CC_Player_CollisionProfileUpdate();

#pragma endregion

};
