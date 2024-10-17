// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "PC_Player.generated.h"


class AGM_TimeArena;
class UInputAction;
class UEnhancedInputLocalPlayerSubsystem;
class UCMC_Player;

struct FDamageEvent;

UCLASS()
class SHOWDOWN_API APC_Player : public APlayerController
{
	GENERATED_BODY()

public:

	UFUNCTION(Server, Reliable, BlueprintCallable)									//Request to spawn character - Server RPC
	void ServerRequestSpawnCharacter();

	void ServerRequestSpawnCharacter_Implementation();								//Request to spawn character - Implementation

	
protected:
	virtual void BeginPlay() override;

	FTimerHandle EnhancedInputTimer;												//Sets timers for binding player inputs

	virtual void OnPossess(APawn* InPawn) override;									//OnPossess- Adds additional functionality
	void DelayedEIBinding();

#pragma region Enhanced Input Bindings

protected:
	virtual void SetupInputComponent() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enhanced Input")			//IMC
		TObjectPtr<class UInputMappingContext> InputMappingContext = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")		//Move
		TObjectPtr<UInputAction> RequestMoveAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")		//Look
		TObjectPtr<UInputAction> RequestLookAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")		//Jump
		TObjectPtr<UInputAction> RequestJumpAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")		//Sprint
		TObjectPtr<UInputAction> RequestSprintAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")		//Crouch
		TObjectPtr<UInputAction> RequestToggleCrouchAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")		//DamageSelf
		TObjectPtr<UInputAction> RequestDamageSelfAction = nullptr;

public:
#pragma endregion

#pragma region Getter and Checker Functions
protected:
	void CheckActiveCharacter();													//Checker function needed for PIE, used when switiching between windows.  ActiveCharacter becomes a nullptr when a window switch occurs. 

	UFUNCTION(BlueprintCallable, Category="PC_Player")								//Getter function - Active Character
	ACHAR_Player* GetActiveCharacter();

	UCMC_Player* GetCustomCharacterMovementComponent();								//Getter function for custom character movement component. 

public:
#pragma endregion

#pragma region Movement Functions and Properties
protected:
	//Movement Functions
	void RequestMove(const FInputActionValue& Value);
	void RequestLook(const FInputActionValue& Value);
	void RequestJump();
	void RequestSprintStart();
	void RequestSprintStop();
	void RequestToggleCrouch();
	void RequestDamageSelf();

	//Movement UPROPERTIES
	UPROPERTY(EditAnywhere, Category = "Look")
	float BaseLookUpRate = 90.0f;
	UPROPERTY(EditAnywhere, Category = "Look")
	float BaseLookRightRate = 90.0f;
public:
#pragma endregion
	
#pragma region HUD
protected:
	UFUNCTION(Client, Reliable)														//Client RPC - Tells client to call ConstructHUDWidget Function
	void ClientConstructHUDWidget();
	
	UFUNCTION(BlueprintNativeEvent, Category="HUD")									//BP NATIVE: Builds the HUD and assigns it to the player's viewport. 
	void ConstructHUDWidget();

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Health")							//BP NATIVE: If the player's health changes, this function will be executed to update the HUD health bar. 
	void SetHealthBarPercentage();

#pragma endregion


#pragma region RPCs
	UFUNCTION(Server, Reliable)
	void ServerDamageSelf(ACHAR_Player* TargetCharacter, float DamageAmount, APC_Player* InstigatingPlayer);


#pragma endregion
protected:
	//GameMode Declaration
	AGM_TimeArena* GameModeRef;

	//Character Declaration
	class ACHAR_Player* ActiveCharacter;

	//PlayerSubsystem Declaration
	UEnhancedInputLocalPlayerSubsystem* PlayerSubsystem = nullptr;

};
