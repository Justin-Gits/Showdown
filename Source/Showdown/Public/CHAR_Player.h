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

#pragma region Character Health

protected:
	
	//Properties
	UPROPERTY(EditDefaultsOnly, Category ="Health")
	float MaxHealth;
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
	float CurrentHealth;

	//Client/Server Health Update functions
	UFUNCTION()						//RepNotify for changes made to current health. 
	void OnRep_CurrentHealth();
	UFUNCTION()
	void OnHealthUpdate();		//Response to health being updated.  Called on the server immediately after modification, and on clients in response to receiving an update. 

public:
	//Getter Function for Max Health
	UFUNCTION(BlueprintPure, Category="Health")
	FORCEINLINE float GetMaxHealth() const {return MaxHealth;}

	//Getter Function for Current Health
	UFUNCTION(BlueprintPure, Category="Health")
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	//SERVER ONLY FUNCTION: Setter function for Current Health.  Clamps value between - and Max Health and calls OnHealthUpdate.
	UFUNCTION(BlueprintCallable, Category="Health")
	void SetCurrentHealth(float healthValue);

	//Event for taking damage.  Overridden from APawn.
	UFUNCTION(BlueprintCallable, Category="Health")
	float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


#pragma endregion

};
