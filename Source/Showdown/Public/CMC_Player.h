// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CMC_Player.generated.h"


UCLASS()
class SHOWDOWN_API UCMC_Player : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
	friend class FSavedMove_My;

#pragma region Defaults
private:
	//The ground speed when running
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CMC_Player", Meta = (AllowPrivateAccess = "true"));
	float RunSpeed = 300.0f;

	//The ground speed when sprinting
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CMC_Player", Meta = (AllowPrivateAccess = "true"));
	float SprintSpeed = 800.0f;

	//The acceleration when running
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CMC_Player", Meta = (AllowPrivateAccess = "true"));
	float RunAcceleration = 300.0f;

	//The acceleration when sprinting
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CMC_Player", Meta = (AllowPrivateAccess = "true"));
	float SprintAcceleration = 2000.0f;


#pragma endregion

#pragma region Sprinting Functions

public:
	//Sets sprinting to either enabled or disabled
	UFUNCTION(BlueprintCallable, Category = "CMC_Player")
	void SetSprinting(bool sprinting);

#pragma endregion

#pragma region Overrides
public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual float GetMaxSpeed() const override;
	virtual float GetMaxAcceleration() const override;
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	
#pragma endregion

#pragma region Compressed Flags
private:
	uint8 WantsToSprint : 1;
#pragma endregion

#pragma region Private Variables
	//True if the sprint key is down
	bool SprintKeyDown = false;

#pragma endregion
};


/*These two classes(FSavedMove_My& FNetworkPredictionData_Client_My) are used by Unreal and it is the way that they handle networked character movement with the concept of "Saved Moves")_
  So, whenever you move on the client (e.g.: Jump), you jump on the client, and then it saves the information about that jump into the FSavedMove_My class, and then it sends that Saved Move
  to the server so it can jump.  The server then tells all of the other clients to perform that move as well. In order to add to this functionality, we need to create our
  own SavedMove class, which we will call FSavedMove_My. 
 */
class FSavedMove_My : public FSavedMove_Character
{
public:

	typedef FSavedMove_Character Super;

	//Resets all saved variables
	virtual void Clear() override;
	//Store input commands in the compressed flags.
	virtual uint8 GetCompressedFlags() const override;
	//This is used to check whether or not two moves can be combined into one. 
	//Basically, you just check to make sure that the saved variables are the same. 
	virtual bool CanCombineWith(const FSavedMovePtr& NewMovePtr, ACharacter* Character, float MaxDelta) const override;
	//Sets up the move before sending it to the server.
	virtual void SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData) override;
	//Sets variables on character movement component before making a predictive correction.
	virtual void PrepMoveFor(class ACharacter* Character) override;

private:
	//Compressed Flags:  Flags which are essentially booleans, that get passed between the client and the server.  This is what keeps the client and server in-sync with one another. 
	//We will have the same flags in our character movement component, just minus the Saved keyword in front of them. 

	//In general, the whole purpose of this FSavedMove class is to take these compressed flags for information about the character (e.g. they want to sprint), and copy them into the
	//SavedWantsToSprint flag, and this flag is what gets sent to the server. 
	uint8 SavedWantsToSprint : 1;
};

//This is basically just a helper class that connects the FSavedMove to the CharacterMovementComponent. 
class FNetworkPredictionData_Client_My : public FNetworkPredictionData_Client_Character
{
public:
	typedef FNetworkPredictionData_Client_Character Super;

	//Constructor
	FNetworkPredictionData_Client_My(const UCharacterMovementComponent& ClientMovement);

	//brief Allocates a new copy of our custom saved move
	virtual FSavedMovePtr AllocateNewMove() override;
};
