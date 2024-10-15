// Fill out your copyright notice in the Description page of Project Settings.


#include "CMC_Player.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"


//UCharacterMovementComponent::UCharacterMovementComponent(const FObjectInitializer& ObjectInitializer)



void UCMC_Player::SetSprinting(bool sprinting)
{
	SprintKeyDown = sprinting;
}

void UCMC_Player::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//Perform local only checks:

	if (GetPawnOwner()->IsLocallyControlled())
	{
		if (SprintKeyDown == true)
		{
			//Only set WantsToSprint to true if the player is moving forward (so that they can't sprint backwards)
			FVector velocity2D = GetPawnOwner()->GetVelocity();
			FVector forward2D = GetPawnOwner()->GetActorForwardVector();
			velocity2D.Z = 0.0f;
			forward2D.Z = 0.0f;
			velocity2D.Normalize();
			forward2D.Normalize();

			if (FVector::DotProduct(velocity2D, forward2D) > 0.5f)
			{
				/*I believe the source of my rubberbanding is the fact that the server is not changing the movement speed when the client presses the sprint button. 
				In order to fix this, I intend to insert an RPC here which tells the server to update this value.*/
				WantsToSprint = true;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("UCMC_Player::TickComponent - Sprint not firing, directional dot product not high enough."));
			}
		}
		else
		{
			WantsToSprint = false;
		}
	}
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}


void UCMC_Player::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	/*  There are 4 custom move flags for us to use.  Below is what each is currently being used for:
			FLAG_Custom_0		=0x10, // Sprinting
			FLAG_Custom_1		=0x20, // Unused
			FLAG_Custom_2		=0x40, // Unused
			FLAG_Custom_3		=0x80, // Unused
	*/

	//Read the values from the compressed flags
	WantsToSprint = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

float UCMC_Player::GetMaxSpeed() const
{
	switch (MovementMode)
	{
		case MOVE_Walking:
		case MOVE_NavWalking:
		{
			if (IsCrouching())
			{
				return MaxWalkSpeedCrouched;		// TODO: I may need to update this for the situations where I am crouching and I want to sprint. 
			}
			else
			{
				if (WantsToSprint)
				{
					return SprintSpeed;
				}
			}
			return RunSpeed;
		}
		case MOVE_Falling:
			return RunSpeed;
		case MOVE_Swimming:
			return MaxSwimSpeed;
		case MOVE_Flying:
			return MaxFlySpeed;
		case MOVE_Custom:
			return MaxCustomMovementSpeed;
		case MOVE_None:
		default:
			return 0.f;
	}
}

float UCMC_Player::GetMaxAcceleration() const
{
	if (IsMovingOnGround())
	{
		if (WantsToSprint)
		{
			return SprintAcceleration;
		}
		return RunAcceleration;
	}

	return Super::GetMaxAcceleration();
}

void FSavedMove_My::Clear()
{
	Super::Clear();

	//Clear all values
	SavedWantsToSprint = 0;
}

uint8 FSavedMove_My::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	/*There are 4 custom move flags for us to use.  Below is what each is currently being used for:
	FLAG_Custom_0		=0x10, // Sprinting
	FLAG_Custom_1		=0x20, // Unused
	FLAG_Custom_2		=0x40, // Unused
	FLAG_Custom_3		=0x80, // Unused
	*/

	//Write to the compressed flags
	if (SavedWantsToSprint)
		Result |= FLAG_Custom_0;

	return Result;
}

bool FSavedMove_My::CanCombineWith(const FSavedMovePtr& NewMovePtr, ACharacter* Character, float MaxDelta) const
{
	//Nothing needed here at the moment, the original purpose was to combine the wall run with the sprint. 	
	return Super::CanCombineWith(NewMovePtr, Character, MaxDelta);
}

//The SetMoveFor and PrepMoveFor functions faciliatate the passing of the compressed flags. 
void FSavedMove_My::SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	UCMC_Player* charMov = Cast<UCMC_Player>(Character->GetCharacterMovement());
	if (charMov)
	{
		//Copy values into the saved move
		SavedWantsToSprint = charMov->WantsToSprint;
	}
	
}

//The SetMoveFor and PrepMoveFor functions faciliatate the passing of the compressed flags. 
void FSavedMove_My::PrepMoveFor(ACharacter* Character)
{
	Super::PrepMoveFor(Character);

	UCMC_Player* charMov = Cast<UCMC_Player>(Character->GetCharacterMovement());
	if (charMov)
	{
		//Copy values out of the saved move
		charMov->WantsToSprint = SavedWantsToSprint;
	}
}

FNetworkPredictionData_Client* UCMC_Player::GetPredictionData_Client() const
{
	if (ClientPredictionData == nullptr)
	{
		//Return our custom client prediction class instead
		UCMC_Player* MutableThis = const_cast<UCMC_Player*>(this);
		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_My(*this);
	}
	
	
	return ClientPredictionData;
}

FNetworkPredictionData_Client_My::FNetworkPredictionData_Client_My(const UCharacterMovementComponent& ClientMovement)
	:Super(ClientMovement)
{
}

FSavedMovePtr FNetworkPredictionData_Client_My::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_My());
}
