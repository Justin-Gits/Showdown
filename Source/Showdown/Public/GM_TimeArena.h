// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GM_TimeArena.generated.h"

UCLASS()
class SHOWDOWN_API AGM_TimeArena : public AGameMode
{
	GENERATED_BODY()
	
public:

UFUNCTION(BlueprintNativeEvent, Category="Server")
void SpawnCharacterForPlayer(APC_Player* PlayerController);

void SpawnCharacterForPlayer_Implementation(APC_Player* PlayerController);

protected:
UPROPERTY()
TArray<APlayerController*> PlayerControllerList;

UFUNCTION()
virtual void PostLogin(APlayerController* NewPlayer) override;

UFUNCTION()
virtual void SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC) override;

UFUNCTION()
virtual void Logout(AController* ExitingController) override;


};
