// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACTOR_BaseWeaponProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class SHOWDOWN_API AACTOR_BaseWeaponProjectile : public AActor
{
	GENERATED_BODY()

private:
	//Sphere Collision Component
	UPROPERTY(VisibleAnywhere, Category=Projectile)
	USphereComponent* BulletCollisionComponent;

	//Projectile Movement Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent * BulletProjectileMovement;
	
#pragma region Defaults
public:
	AACTOR_BaseWeaponProjectile();

	//Network replication
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

#pragma endregion

#pragma region Properties
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Projectile")
	UStaticMeshComponent* BulletProjectileMesh;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	//float BulletProjectileRadius;

#pragma endregion

#pragma region Setter and Getter Functions
public:
	USphereComponent* GetBulletCollisionComponent() const {	return BulletCollisionComponent;}
	UProjectileMovementComponent* GetBulletProjectileMovement() const {	return BulletProjectileMovement;}

#pragma endregion

#pragma region Collision Functions
public:
	
	//Called when the projectile hits something
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

#pragma endregion

#pragma region Networking

	bool IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const override;

#pragma endregion
};
