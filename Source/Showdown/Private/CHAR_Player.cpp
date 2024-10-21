// Fill out your copyright notice in the Description page of Project Settings.


#include "CHAR_Player.h"
#include "PC_Player.h"
#include "CMC_Player.h"
#include "SMC_BaseWeapon.h"
#include "Net/UnrealNetwork.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/DamageType.h"
#include "Engine/Engine.h"

// Sets default values
ACHAR_Player::ACHAR_Player(const class FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer.SetDefaultSubobjectClass<UCMC_Player>(ACharacter::CharacterMovementComponentName))
{
 	//Default Character Properties
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth-20.0f;
	
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Network Replication
	bReplicates = true;
	SetReplicateMovement(true);

}

void ACHAR_Player::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicated Properties
	DOREPLIFETIME(ACHAR_Player, CurrentHealth);

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

#pragma region Health

void ACHAR_Player::OnHealthUpdate()
{
	//Client Functionality
	if (IsLocallyControlled())
	{
		APC_Player* ReferencePlayer = Cast<APC_Player>(GetController());
		ReferencePlayer->SetHealthBarPercentage();
		FString healthMessage = FString::Printf(TEXT("You now have %f health remaining."), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

		if (CurrentHealth <= 0.0f)
		{
			FString deathMessage = FString::Printf(TEXT("You have perished."), CurrentHealth);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);
		}
	}

	//Server Functionality
	if (GetLocalRole() == ROLE_Authority)
	{
		FString healthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);
	}
}



float ACHAR_Player::GetCurrentHealthPercentage() const
{
	float CurrentHealthPercentage = CurrentHealth / MaxHealth;
	//UE_LOG(LogTemp, Warning, TEXT("Current Health Percentage = %f"), CurrentHealthPercentage);
	return CurrentHealthPercentage;
}

void ACHAR_Player::SetCurrentHealth(float healthValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHealth = FMath::Clamp(healthValue, 0.f, MaxHealth);
		OnHealthUpdate();
	}
}

float ACHAR_Player::TakeDamage(float DamageTaken, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// TODO:  The Super implementation is not here.  If i get more sophisticated with the way my damage is applied, then I may have to update this. 
	float damageApplied = CurrentHealth - DamageTaken;
	SetCurrentHealth(damageApplied);
	return damageApplied;
}

void ACHAR_Player::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}

#pragma endregion


#pragma region Weapon Firing Mechanics

void ACHAR_Player::RequestFireWeapon()
{
	//FString requestFireWeapon = FString::Printf(TEXT("CHAR_Player::RequestFireWeapon() - CHARACTER BANG!"));
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, requestFireWeapon);
	CheckIfWeaponEquipped();
}

void ACHAR_Player::CheckIfWeaponEquipped()
{
	USMC_BaseWeapon* EquippedWeaponSMC = FindComponentByClass<USMC_BaseWeapon>();
	if (EquippedWeaponSMC)
	{
		FString weaponEquipped = FString::Printf(TEXT("CHAR_Player::CheckIfWeaponEquipped() - You have a weapon, BANG!"));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, weaponEquipped);
	}
	else
	{
		FString noWeaponEquipped = FString::Printf(TEXT("CHAR_Player::CheckIfWeaponEquipped() - STOP - No Weapon Equipped!"));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, noWeaponEquipped);
	}
}

#pragma endregion



