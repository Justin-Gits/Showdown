// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "TimerManager.h"
#include "PC_Player.generated.h"


class AGM_TimeArena;
class UInputAction;
class UEnhancedInputLocalPlayerSubsystem;


UCLASS()
class SHOWDOWN_API APC_Player : public APlayerController
{
	GENERATED_BODY()

public:
	//Request to spawn character - Server RPC
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRequestSpawnCharacter();

	//Request to spawn character - Implementation
	void ServerRequestSpawnCharacter_Implementation();

	

protected:
	virtual void BeginPlay() override;

	//Sets timers for binding player inputs
	FTimerHandle EnhancedInputTimer;

	//On Pawn Possession 
	virtual void OnPossess(APawn* InPawn) override;
	void DelayedEIBinding();

	//Enhanced Input Setup
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

	//Checker function needed for PIE, used when switiching between windows.  ActiveCharacter becomes a nullptr when a window switch occurs. 
	void CheckActiveCharacter();
	
	//Movement Functions
	void RequestMove(const FInputActionValue& Value);
	void RequestLook(const FInputActionValue& Value);
	void RequestJump();
	//void RequestSprint();

	//Movement UPROPERTIES
	UPROPERTY(EditAnywhere, Category="Look")
	float BaseLookUpRate = 90.0f;
	UPROPERTY(EditAnywhere, Category="Look")
	float BaseLookRightRate = 90.0f;

	//GameMode Declaration
	AGM_TimeArena* GameModeRef;

	//Character Declaration
	class ACHAR_Player* ActiveCharacter;

	//PlayerSubsystem Declaration
	UEnhancedInputLocalPlayerSubsystem* PlayerSubsystem = nullptr;

};
