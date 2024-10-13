// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CHAR_Player.generated.h"

class UCMC_Player;

UCLASS()
class SHOWDOWN_API ACHAR_Player : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACHAR_Player(const class FObjectInitializer& ObjectInitializer);

	//Network replication
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Gets the custom character movement component - CMC_Player. 
	UFUNCTION(BlueprintCallable, Category = "Movement")
	UCMC_Player* GetCMC_Player() const;

#pragma region Character Movement

	void SprintStart();
	void SprintStop();
#pragma endregion

#pragma region RPCs
protected:
UFUNCTION(Server, Reliable, WithValidation)
void ServerHandleSprintStart(UCMC_Player* CMCReference);

UFUNCTION(Server, Reliable)
void ServerHandleSprintStop(UCMC_Player* CMCReference);

#pragma endregion

};
