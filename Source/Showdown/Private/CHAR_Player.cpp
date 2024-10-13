// Fill out your copyright notice in the Description page of Project Settings.


#include "CHAR_Player.h"
#include "CMC_Player.h"

// Sets default values
ACHAR_Player::ACHAR_Player(const class FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer.SetDefaultSubobjectClass<UCMC_Player>(ACharacter::CharacterMovementComponentName))
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
	NetUpdateFrequency = 120.0f;

	
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

UCMC_Player* ACHAR_Player::GetCMC_Player() const
{
	return Cast<UCMC_Player>(GetCharacterMovement());
}

#pragma region Character Movement
void ACHAR_Player::SprintStart()
{
	UCMC_Player* CMC = GetCMC_Player();
	CMC->SetSprinting(true);
	if (!HasAuthority())
	{
		ServerHandleSprintStart(CMC);
	}

}

void ACHAR_Player::SprintStop()
{
	UCMC_Player* CMC = GetCMC_Player();
	CMC->SetSprinting(false);
	if (!HasAuthority())
	{
		ServerHandleSprintStop(CMC);
	}
}
#pragma endregion

#pragma region RPCs
void ACHAR_Player::ServerHandleSprintStart_Implementation(UCMC_Player* CMCReference)
{
	CMCReference->SetSprinting(true);
}

bool ACHAR_Player::ServerHandleSprintStart_Validate(UCMC_Player* CMCReference)
{
	// TODO:  Insert checks to prevent sprinting when there isn't enough energy
	return true;
}

void ACHAR_Player::ServerHandleSprintStop_Implementation(UCMC_Player* CMCReference)
{
	CMCReference->SetSprinting(false);
}

#pragma endregion

