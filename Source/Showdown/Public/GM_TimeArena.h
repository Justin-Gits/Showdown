// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GM_TimeArena.generated.h"

USTRUCT(BlueprintType)
struct FWeaponStats
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float Damage;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float BulletSpeed;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float FireType;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float FireRate;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float ReloadTime;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float MagazineCapacity;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float AimTime;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float Recoil;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float BulletSpread;
};

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
