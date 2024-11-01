// Fill out your copyright notice in the Description page of Project Settings.


//Custom Classes
#include "CHAR_Player.h"
#include "PC_Player.h"
#include "CMC_Player.h"
#include "SMC_BaseWeapon.h"
#include "CC_Player.h"

//Engine Functionality
#include "Engine/EngineTypes.h"
#include "GameFramework/DamageType.h"
#include "Engine/Engine.h"
#include "Components/CapsuleComponent.h"

//Replication
#include "Net/UnrealNetwork.h"

// Sets Snapshot Mode default values
ACHAR_Player::ACHAR_Player(const class FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer
		.SetDefaultSubobjectClass<UCMC_Player>(ACharacter::CharacterMovementComponentName)
		.SetDefaultSubobjectClass<UCC_Player>(ACharacter::CapsuleComponentName)
		)
{
 	//Default Character Spawn Parameters
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;

	//Collision Profile - Snapshot
	GetCMC_Player()->GravityScale = 0.0f;
	CC_PlayerCapsuleComponent = Cast<UCC_Player>(GetCapsuleComponent());
	CC_PlayerCapsuleComponent->SetCollisionProfileName("NoCollision");
	
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

void ACHAR_Player::Jump()
{
	if (HasAuthority())
	{
		Super::Jump();
	}
	else if (IsLocallyControlled())
	{
		Super::Jump();
		//Jump();

		//Debug print out gravity and collision information:
		float GravityValue = GetCMC_Player()->GravityScale = 1.0f;
		FString gravityMessage = FString::Printf(TEXT("ACHAR_Player::Jump() - Gravity = %f"),GravityValue);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, gravityMessage);

		FName CollisionProfileName = CC_PlayerCapsuleComponent->GetCollisionProfileName();
		//FString collisionMessage = FString::Printf(TEXT("ACHAR_Player::Jump() - CollisionProfileName = %s"), CollisionProfileName.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, CollisionProfileName.ToString());

		
		ServerJump();
	}
}

void ACHAR_Player::ServerJump_Implementation()
{
	if (HasAuthority())
	{
		Super::Jump();
		FString clientJump = FString::Printf(TEXT("ACHAR_Player::ServerJump_Implementation() - RPC to Jump!"));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, clientJump);
	}
	
}

#pragma region Health

void ACHAR_Player::OnHealthUpdate()
{
	APC_Player* ReferencePlayer = Cast<APC_Player>(GetController());
	//Client Functionality
	if (IsLocallyControlled())
	{
		ReferencePlayer->SetHealthBarPercentage();
		FString healthMessage = FString::Printf(TEXT("You now have %f health remaining."), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

		//if (CurrentHealth <= 0.0f)
		//{
		//	FString deathMessage = FString::Printf(TEXT("You have perished."), CurrentHealth);
		//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);
		//}
	}

	//Server Functionality
	if (GetLocalRole() == ROLE_Authority)
	{
		FString healthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

		if (CurrentHealth <= 0.0f)
		{
			FString deathMessage = FString::Printf(TEXT("You have perished."));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);
			Destroy();
			//ReferencePlayer->RequestRespawn();
		}
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
	if (bSnapshotSetup == true)
	{
		//Ignore health update
		return;
	}
	else
	{
		OnHealthUpdate();
	}

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
		//FString weaponEquipped = FString::Printf(TEXT("CHAR_Player::CheckIfWeaponEquipped() - You have a weapon, BANG!"));
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, weaponEquipped);
		EquippedWeaponSMC->NetworkRequestFireWeapon(this);
	}
	else
	{
		FString noWeaponEquipped = FString::Printf(TEXT("CHAR_Player::CheckIfWeaponEquipped() - STOP - No Weapon Equipped!"));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, noWeaponEquipped);
	}
}

void ACHAR_Player::InitializeSnapshotSpawn(float SnapshotHealthInput, float SnapshotAmmoInput, FVector SnapshotVelocityInput)
{
	bSnapshotSetup = true;
	SnapshotVelocity = SnapshotVelocityInput;
	CurrentHealth = SnapshotHealthInput;
	//Snapshot Ammo Input goes here
	GetCMC_Player()->GravityScale = 0.0f;
	

	//More important logic will be placed in LeavingSnapshotMode 

}

void ACHAR_Player::PossessedBy(AController* PlayerController)
{
	Super::PossessedBy(PlayerController);
	LeavingSnapshotMode();


}

void ACHAR_Player::LeavingSnapshotMode()
{
	GetCMC_Player()->GravityScale = 1.0f;
	if (CC_PlayerCapsuleComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ACHAR_Player::LeavingSnapshotMode - CC_PlayerCapsuleComponent = nullptr"));
	}
	else
	{
		CC_PlayerCapsuleComponent->SetCollisionProfileName("Pawn");
	}
		//GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
	if (SnapshotVelocity == FVector::ZeroVector)
	{
		UE_LOG(LogTemp, Warning, TEXT("ACHAR_Player::LeavingSnapshotMode - ZeroVector for Snapshot Velocity"));
	}
	else
	{
		GetCMC_Player()->Velocity = SnapshotVelocity;
	}
	bSnapshotSetup = false;
	OnHealthUpdate();

}

#pragma endregion




