// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CHAR_Player.generated.h"

class UCMC_Player;
class APC_Player;
class UCC_Player;
struct FDamageEvent;

UCLASS()
class SHOWDOWN_API ACHAR_Player : public ACharacter
{
	GENERATED_BODY()

	friend class AGM_TimeArena;

#pragma region Defaults
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

#pragma endregion

#pragma region Controller and Movement Component Interface

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Gets the custom character movement component - CMC_Player. 
	UFUNCTION(BlueprintCallable, Category = "Movement")
	UCMC_Player* GetCMC_Player() const;

#pragma endregion

#pragma region Movement

	virtual void Jump() override;

	UFUNCTION(Server, Reliable)
	void ServerJump();
	//virtual void StopJumping() override;

#pragma endregion

#pragma region Character Health

protected:
	
	//Properties
	UPROPERTY(EditDefaultsOnly, Category ="Health", BlueprintReadOnly)
	float MaxHealth;
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth, BlueprintReadOnly)
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

	//Getter Function for Percentage of Health
	UFUNCTION(BlueprintPure, Category="Health")
	float GetCurrentHealthPercentage() const;
	
	//SERVER ONLY FUNCTION: Setter function for Current Health.  Clamps value between - and Max Health and calls OnHealthUpdate.
	UFUNCTION(BlueprintCallable, Category="Health")
	void SetCurrentHealth(float healthValue);

	//Taking damage.  Overridden from APawn.
	UFUNCTION(BlueprintCallable, Category = "Health")
	float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

#pragma endregion

#pragma region Weapon Firing Mechanics
protected:

public:
	void RequestFireWeapon();
	void CheckIfWeaponEquipped();

#pragma endregion

#pragma region Spawning

protected:

	UPROPERTY()
	UCC_Player* CC_PlayerCapsuleComponent;
	
	UPROPERTY()
	bool bSnapshotSetup;

	UPROPERTY()
	FVector SnapshotVelocity;
	
	UFUNCTION(BlueprintCallable, Category="Custom Spawn")
	void InitializeSnapshotSpawn(float SnapshotHealthInput, float SnapshotAmmoInput, FVector SnapshotVelocityInput);

	virtual void PossessedBy(AController* PlayerController) override;
	
	UFUNCTION(BlueprintCallable, Category="Custom Spawn")
	void LeavingSnapshotMode();

#pragma endregion
};
