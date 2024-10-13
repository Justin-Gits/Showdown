// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Player.h"
#include "GM_TimeArena.h"
#include "CHAR_Player.h"
#include "CMC_Player.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"
#include "Engine/LocalPlayer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

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
	Super::OnPossess(InPawn);
	ActiveCharacter = Cast<ACHAR_Player>(InPawn);
	ensureMsgf(ActiveCharacter != nullptr, TEXT("APC_Player::OnPossess - Active Character = nullptr"));
}

void APC_Player::DelayedEIBinding()
{
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (LocalPlayer && IsLocalPlayerController())						// TODO:  I also checked to make sure that the player is a LocalPlayerController prior to performing the enhanced input bindings to ensure that they don't get binded twice.  However, this may be an error so if i'm getting issues with it this is the place to start looking. 
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
	}
}

void APC_Player::CheckActiveCharacter()
{
	if (ActiveCharacter == nullptr) {
	ActiveCharacter = Cast<ACHAR_Player>(GetCharacter());
	}
}

UCMC_Player* APC_Player::GetCustomCharacterMovementComponent()
{
	ACHAR_Player* ActivePlayer = Cast<ACHAR_Player>(GetCharacter());
	UCMC_Player* CMC = ActivePlayer->GetCMC_Player();
	ensureMsgf(CMC != nullptr, TEXT("APC_Player::GetCustomCharacterMovementComponent - Getting Custom Character Movement Component returns nullptr"));
	return CMC;
}

void APC_Player::RequestMove(const FInputActionValue& Value)
{
	CheckActiveCharacter();
	FVector2D MovementVector = Value.Get<FVector2D>();

	FRotator const ControlSpaceRot = GetControlRotation();
	ActiveCharacter->AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::X), MovementVector.X);
	ActiveCharacter->AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::Y), MovementVector.Y);

	//UE_LOG(LogTemp, Warning, TEXT("APC_Player::RequestMove - Executed"));
}

void APC_Player::RequestLook(const FInputActionValue& Value)
{
	CheckActiveCharacter();
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	ActiveCharacter->AddControllerYawInput(LookAxisVector.X);
	ActiveCharacter->AddControllerPitchInput(-1.0f*LookAxisVector.Y);

	//UE_LOG(LogTemp, Warning, TEXT("APC_Player::RequestLook - Executed:"));
}

void APC_Player::RequestJump()
{
	CheckActiveCharacter();
	//UE_LOG(LogTemp, Warning, TEXT("APC_Network_Multiplayer::RequestJump Executed"));
	ActiveCharacter->Jump();
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














