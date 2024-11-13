// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "CHAR_Player.generated.h"

class UCMC_Player;
class APC_Player;
struct FDamageEvent;

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
	UPROPERTY(EditDefaultsOnly, Category ="Health", BlueprintReadOnly)
	float MaxHealth;
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth, BlueprintReadOnly)
	float CurrentHealth;

	//Client/Server Health Update functions
	UFUNCTION()						//RepNotify for changes made to current health. 
	void OnRep_CurrentHealth();
	UFUNCTION()
	void OnHealthUpdate();		//Response to health being updated.  Called on the server immediately after modification, and on clients in response to receiving an update. 
	UFUNCTION(Server, Reliable)
	void ServerDestroyCharacter(APC_Player* TargetPlayer);


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

public:
	void RequestFireWeapon();
	void CheckIfWeaponEquipped();

#pragma endregion

#pragma region Character Management
public:	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSnapshotPossessionEvent();

	virtual void PossessedBy(AController* NewController) override;

#pragma endregion

#pragma region Snapshot Spawn Points
public:
	void RequestSetAdditionalSpawnParameters(FVector InputVelocity, float InputHealth, float InputAmmo);
protected:
	UFUNCTION()
	void SetAdditionalSpawnParameters(FVector InputVelocity, float InputHealth, float InputAmmo);

	UPROPERTY()
	FVector SnapshotVelocity;

	UPROPERTY()
	float SnapshotAmmo;


#pragma endregion


};
