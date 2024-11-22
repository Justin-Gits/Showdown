// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Player.h"
#include "PS_Player.h"
#include "GM_TimeArena.h"
#include "CHAR_Player.h"
#include "CMC_Player.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"
#include "Engine/LocalPlayer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


void APC_Player::BeginPlay()
{
	Super::BeginPlay();

	GameModeRef = Cast<AGM_TimeArena>(GetWorld()->GetAuthGameMode());		// TODO:  This may not work on the client since it mentions that the client will return null.  I may need to do an RPC.  

	if (IsLocalPlayerController())
	{
		ServerRequestSpawnCharacter();
	}
}

void APC_Player::OnPossess(APawn* InPawn)
{
	if (HasAuthority())
	{
		Super::OnPossess(InPawn);
		UE_LOG(LogTemp, Warning, TEXT("APC_Player::OnPossess - Possession Occurred."));
		ActiveCharacter = Cast<ACHAR_Player>(InPawn);
		ensureMsgf(ActiveCharacter != nullptr, TEXT("APC_Player::OnPossess - Active Character = nullptr"));
		if (GetLocalRole() == ROLE_Authority)
		{
			if (IsLocalController())
			{
				ConstructHUDWidget();
			}
			else
			{
				ClientConstructHUDWidget();
			}
		}
	}
}

void APC_Player::OnUnPossess()
{
	if (HasAuthority())
	{
		Super::OnUnPossess();
		//ServerRequestSpawnCharacter();
		if (IsLocalController())
		{
			UpdateHUD = false;
			DestroyHUDWidget();
			UE_LOG(LogTemp, Warning, TEXT("APC_Player::OnUnPossess() - Host no longer possess a controller."));
			RequestSearchForSnapshot();
		}
		else
		{
			UpdateHUD = false;
			ClientDestroyHUDWidget();
			RequestSearchForSnapshot();
			//Need to update with client side logic once we get host logic to work. 
		}

	}
}

void APC_Player::RequestSearchForSnapshot()
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("APC_Player::RequestSearchForSnapshot - Executing."));
		ExecuteSearchForSnapshot();
	}
}

void APC_Player::ExecuteSearchForSnapshot()
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("APC_Player::ExecuteSearchForSnapshot - Executing."));
		ReferencePlayerState = GetPlayerState<APS_Player>();
		if (IsValid(ReferencePlayerState))
		{
			ACHAR_Player* NextSpawn = ReferencePlayerState->GetSnapshotSpawn();
			if (NextSpawn == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("APC_Player::ExecuteSearchForSnapshot - NextSpawn = nullptr!"));
			}
			else
			{
				FString NextSpawnName = NextSpawn->GetName();
				UE_LOG(LogTemp, Warning, TEXT("Next Spawn Name: %s"), *NextSpawnName);
				UpdateHUD = true;
				if (GetPawn())
				{
					UE_LOG(LogTemp, Warning, TEXT("Player controller already possesses something else, unpossessing now!"));
					UnPossess();
				}
				GameModeRef->PossessCharacterUsingPlayer(NextSpawn, this);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("APC_Player::ExecuteSearchForSnapshot - Reference Player State = nullptr."));
		}
		
	}
}

void APC_Player::DestroyPossessedCharacter(ACHAR_Player* TargetCharacter)
{
	if (HasAuthority())
	{
		UnPossess();
		ReferencePlayerState = GetPlayerState<APS_Player>();
		ReferencePlayerState->RequestRestartSnapshotTimer();
		TargetCharacter->Destroy();
	}
}



void APC_Player::DelayedEIBinding()
{
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (LocalPlayer && IsLocalPlayerController())						
	{
		PlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
		ensureMsgf(PlayerSubsystem != nullptr, TEXT("APC_Player::DelayedEIBinding - Returned nullptr for the local subsystem"));
		PlayerSubsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void APC_Player::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	GetWorld()->GetTimerManager().SetTimer(EnhancedInputTimer, this, &APC_Player::DelayedEIBinding, 0.2f, false); //Required so the client has a chance to join prior to binding the controls. 

	if (EnhancedInputComponent)
	{
		if (RequestMoveAction)
		{
			EnhancedInputComponent->BindAction(RequestMoveAction, ETriggerEvent::Triggered, this, &APC_Player::RequestMove);
		}
		if (RequestLookAction)
		{
			EnhancedInputComponent->BindAction(RequestLookAction, ETriggerEvent::Triggered, this, &APC_Player::RequestLook);
		}
		if (RequestJumpAction)
		{
			EnhancedInputComponent->BindAction(RequestJumpAction, ETriggerEvent::Started, this, &APC_Player::RequestJump);
		}
		if (RequestSprintAction)
		{
			EnhancedInputComponent->BindAction(RequestSprintAction, ETriggerEvent::Started, this, &APC_Player::RequestSprintStart);
			EnhancedInputComponent->BindAction(RequestSprintAction, ETriggerEvent::Completed, this, &APC_Player::RequestSprintStop);
		}
		if (RequestToggleCrouchAction)
		{
			EnhancedInputComponent->BindAction(RequestToggleCrouchAction, ETriggerEvent::Started, this, &APC_Player::RequestToggleCrouch);
		}
		if (RequestDamageSelfAction)
		{
			EnhancedInputComponent->BindAction(RequestDamageSelfAction, ETriggerEvent::Started, this, &APC_Player::RequestDamageSelf);
		}
		if (RequestFireWeaponAction)
		{
			EnhancedInputComponent->BindAction(RequestFireWeaponAction, ETriggerEvent::Started, this, &APC_Player::RequestFireWeapon);
		}
	}
}

void APC_Player::CheckActiveCharacter()
{
	if (ActiveCharacter == nullptr) {
	ActiveCharacter = Cast<ACHAR_Player>(GetCharacter());
	}
}

ACHAR_Player* APC_Player::GetActiveCharacter()
{
	if (ActiveCharacter == nullptr) {
		ActiveCharacter = Cast<ACHAR_Player>(GetCharacter());
		if (ActiveCharacter == nullptr)
		{
			return nullptr;
		}
		else
		{
			return ActiveCharacter;
		}
	}
	else 
	{
		return ActiveCharacter;
	}
}

UCMC_Player* APC_Player::GetCustomCharacterMovementComponent()
{
	ACHAR_Player* ActivePlayer = Cast<ACHAR_Player>(GetCharacter());
	UCMC_Player* CMC = ActivePlayer->GetCMC_Player();
	ensureMsgf(CMC != nullptr, TEXT("APC_Player::GetCustomCharacterMovementComponent - Getting Custom Character Movement Component returns nullptr"));
	return CMC;
}

#pragma region Character Movement and Actions

bool APC_Player::PossessingCharacter()
{
	if (IsLocalController())
	{
		if (GetCharacter() == nullptr)
		{
			return false;
		}
		else 
		{
			return true;
		}
	}
	return false;
}

void APC_Player::RequestMove(const FInputActionValue& Value)
{
	if (PossessingCharacter())
	{
//		UE_LOG(LogTemp, Warning, TEXT("APC_Player::RequestMove - Has Occurred."));
		ActiveCharacter = GetActiveCharacter();
		FVector2D MovementVector = Value.Get<FVector2D>();
		FRotator const ControlSpaceRot = GetControlRotation();
		ActiveCharacter->AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::X), MovementVector.X);
		ActiveCharacter->AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::Y), MovementVector.Y);
	}
	else
	{
		return;
	}
}

void APC_Player::RequestLook(const FInputActionValue& Value)
{
	if (PossessingCharacter())
	{
		ActiveCharacter = GetActiveCharacter();
		FVector2D LookAxisVector = Value.Get<FVector2D>();
		ActiveCharacter->AddControllerYawInput(LookAxisVector.X);
		ActiveCharacter->AddControllerPitchInput(-1.0f * LookAxisVector.Y);
	}
	else
	{
		return;
	}
}

void APC_Player::RequestJump()
{
	if (PossessingCharacter())
	{
		ActiveCharacter = GetActiveCharacter();
		ActiveCharacter->Jump();
	}
	else
	{
		return;
	}
}

void APC_Player::RequestSprintStart()
{
	UCMC_Player* PlayerCustomMovementComponent = GetCustomCharacterMovementComponent();
	PlayerCustomMovementComponent->SetSprinting(true);
	UE_LOG(LogTemp, Warning, TEXT("Sprint Start"));
}

void APC_Player::RequestSprintStop()
{
	UCMC_Player* PlayerCustomMovementComponent = GetCustomCharacterMovementComponent();
	PlayerCustomMovementComponent->SetSprinting(false);
	UE_LOG(LogTemp, Warning, TEXT("Sprint Stop"));
}

void APC_Player::RequestToggleCrouch()
{
	if (PossessingCharacter())
	{
		ActiveCharacter = GetActiveCharacter();
		if (ActiveCharacter->bIsCrouched)
		{
			ActiveCharacter->UnCrouch();
		}
		else
		{
			ActiveCharacter->Crouch();
		}
	}
	else
	{
		return;
	}
}

void APC_Player::RequestDamageSelf()							// TODO:  Should remove this debug functionality prior to game release. 
{
	if (PossessingCharacter())
	{
		ActiveCharacter = GetActiveCharacter();
		float DamageAmount = 10.0f;
		APC_Player* InstigatingPlayer = this;
		if (HasAuthority())
		{
			UGameplayStatics::ApplyDamage(ActiveCharacter, DamageAmount, this, ActiveCharacter, UDamageType::StaticClass());
		}
		else
		{
			ServerDamageSelf(ActiveCharacter, DamageAmount, InstigatingPlayer);
		}
	}
	else
	{
		return;
	}
}

void APC_Player::RequestFireWeapon()
{
	if (PossessingCharacter())
	{
		ActiveCharacter = GetActiveCharacter();
		ActiveCharacter->RequestFireWeapon();
	}
	else
	{
		return;
	}
}

#pragma endregion

#pragma region HUD

void APC_Player::ConstructHUDWidget_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("APC_Player::ConstructHUDWidget_Implementation() - Default Implementation Occurred for this Blueprint Native Event."));
}

void APC_Player::DestroyHUDWidget_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("APC_Player:::DestroyHUDWidget_Implementation() - Default Implementation Occurred for this Blueprint Native Event."));
}

void APC_Player::SetHealthBarPercentage_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("APC_Player::SetHealthBarPercentage_Implementation() - Default Implementation Occurred for this Blueprint Native Event."));
}

#pragma endregion

#pragma region RPCs
void APC_Player::ServerRequestSpawnCharacter_Implementation()
{
	//Ensure that we are on the server:
	if (HasAuthority())
	{
		AGM_TimeArena* CurrentGameMode = Cast<AGM_TimeArena>(GetWorld()->GetAuthGameMode());
		if (CurrentGameMode)
		{
			CurrentGameMode->SpawnCharacterForPlayer(this);
		}
	}
}

void APC_Player::ServerDamageSelf_Implementation(ACHAR_Player* TargetCharacter, float DamageAmount, APC_Player* InstigatingPlayer)
{
	UGameplayStatics::ApplyDamage(TargetCharacter, DamageAmount, this, TargetCharacter, UDamageType::StaticClass());
}

void APC_Player::ServerRequestSearchForSnapshot_Implementation()
{
	ExecuteSearchForSnapshot();
}


void APC_Player::ClientConstructHUDWidget_Implementation()
{
	ensureMsgf(IsLocalController() == true, TEXT("APC_Player::ClientConstructHUDWidget_Implementation() - Still isn't a local player controller..."));
	if (IsLocalController())
	{
		ConstructHUDWidget();
	}
}
void APC_Player::ClientDestroyHUDWidget_Implementation()
{
	UpdateHUD = false;
	DestroyHUDWidget();
}

#pragma endregion


