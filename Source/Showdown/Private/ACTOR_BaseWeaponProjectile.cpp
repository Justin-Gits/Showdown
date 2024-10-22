// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTOR_BaseWeaponProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CHAR_Player.h"
#include "Engine/Engine.h"


//Note - Defaults (Constructor) contains a lot of the collision/projectile movement properties. 
#pragma region Defaults				

AACTOR_BaseWeaponProjectile::AACTOR_BaseWeaponProjectile()
{
	
	//Generates a sphere as a collision representation
	BulletCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("BulletSphereComp"));
	BulletCollisionComponent->InitSphereRadius(5.0f);
	BulletCollisionComponent->BodyInstance.SetCollisionProfileName("Projectile");
	BulletCollisionComponent->OnComponentHit.AddDynamic(this, &AACTOR_BaseWeaponProjectile::OnHit);

	//Ensures that players cannot walk on projectiles
	BulletCollisionComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	BulletCollisionComponent->CanCharacterStepUpOn = ECB_No;

	//Sets collision component as root component
	RootComponent = BulletCollisionComponent;

	//	//Generate a simple static mesh for the projectile
	BulletProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletProjectileMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (MeshAsset.Succeeded())
	{
		BulletProjectileMesh->SetStaticMesh(MeshAsset.Object);
	}
	BulletProjectileMesh->SetupAttachment(RootComponent);

	//Sets BulletProjectileMovementComponent to govern this projectile's movement
	BulletProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BulletProjectileComp"));
	BulletProjectileMovement->UpdatedComponent = BulletCollisionComponent;
	BulletProjectileMovement->InitialSpeed = 3000.0f;			// TODO: Need to link this information to the baseWeapon. Each weapon should spawn their own type of projectile which has the apporpriate damage/speed information. 
	BulletProjectileMovement->MaxSpeed = 3000.0f;
	BulletProjectileMovement->bRotationFollowsVelocity = true;
	BulletProjectileMovement->bShouldBounce = false;

	//Sets default despawn timer
	InitialLifeSpan = 10.0f;

}

// Called when the game starts or when spawned
void AACTOR_BaseWeaponProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

#pragma endregion

#pragma region Collision Functions

void AACTOR_BaseWeaponProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	/*Our goal is to have the projectile deal damage if it hits another character.  If it isn't another player, it gets destroyed (for now). */
	
	if (OtherActor != nullptr && OtherActor->IsA(ACHAR_Player::StaticClass()))
	{
		FString hitCharacter = FString::Printf(TEXT("ACTOR_BaseWeaponProjectile::OnHit - Hit another character class"));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, hitCharacter);
		Destroy();
	}
	else
	{
		FString hitNothing = FString::Printf(TEXT("ACTOR_BaseWeaponProjectile::OnHit - Hit Nothing"));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, hitNothing);
		Destroy();
	}
}

#pragma endregion

// I don't think that the projectile needs to evaluate tick functions
//PrimaryActorTick.bCanEverTick = true;