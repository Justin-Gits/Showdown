// Fill out your copyright notice in the Description page of Project Settings.


#include "CMC_Player.h"

void UCMC_Player::SetSprinting(bool sprinting)
{
	//SprintKeyDown = sprinting;
}


uint8 FSavedMove_My::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	/*There are 4 custom move flags for us to use.  Below is what each is currently being used for:
	FLAG_Custom_0		=0x10, // Sprinting
	FLAG_Custom_1		=0x20, // Unused
	FLAG_Custom_2		=0x40, // Ununsed
	FLAG_Custom_3		=0x80, // Unused
	*/

	//Write to the compressed flags
	if (SavedWantsToSprint)
		Result |= FLAG_Custom_0;

	return uint8();
}

//The SetMoveFor and PrepMoveFor functions faciliatate the passing of the compressed flags. 
void FSavedMove_My::SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);
/*
	UCMC_Player* charMov = static_cast<UCMC_Player*>(Character->GetCharacterMovement());
	if (charMov)
	{
		//Copy values into the saved move
		SavedWantsToSprint = charMov->WantsToSprint;
	}
	*/
}

//The SetMoveFor and PrepMoveFor functions faciliatate the passing of the compressed flags. 
void FSavedMove_My::PrepMoveFor(ACharacter* Character)
{

}
