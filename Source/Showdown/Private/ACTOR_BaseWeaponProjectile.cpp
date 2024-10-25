// Fill out your copyright notice in the Description page of Project Settings.

//Custom Classes
#include "ACTOR_BaseWeaponProjectile.h"
#include "CHAR_Player.h"
#include "PC_Player.h"
//Engine Functionality
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
//Projectile sphere
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
//Networking
#include "Net/UnrealNetwork.h"



//Note - Defaults (Constructor) contains a lot of the collision/projectile movement properties. 
#pragma region Defaults				

AACTOR_BaseWeaponProjectile::AACTOR_BaseWeaponProjectile()
{
	//Generates a sphere as a collision representation
	BulletCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("BulletSphereComp"));
	//BulletCollisionComponent->InitSphereRadius(BulletProjectileMeshRadius);
	BulletCollisionComponent->SetCollisionProfileName("Destructible");
	BulletCollisionComponent->SetNotifyRigidBodyCollision(true);
	BulletCollisionComponent->OnComponentHit.AddDynamic(this, &AACTOR_BaseWeaponProjectile::OnHit);
	BulletCollisionComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	BulletCollisionComponent->CanCharacterStepUpOn = ECB_No;
	
	//Sets collision component as root component
	RootComponent = BulletCollisionComponent;
	
	
	//Create the basic static mesh
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

	//Initialize Projectile Damage
	BulletProjectileDamage = 10.0f;

	//Network Replication
	bReplicates = true;
	SetReplicateMovement(true);									

	//Instigator Controller Refs
	
}

void AACTOR_BaseWeaponProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME_CONDITION(AACTOR_BaseWeaponProjectile, bReplicates, COND_SkipOwner);
	//Replicated Properties
	//Currently none. 

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
	if (HasAuthority())
	{
		APC_Player* AttackingPlayerController = Cast<APC_Player>(GetInstigatorController());
		ACHAR_Player* AttackingCharacter = Cast<ACHAR_Player>(AttackingPlayerController->GetPawn());
		if (OtherComp != nullptr && OtherComp->IsA(UCapsuleComponent::StaticClass()))
		{
			AActor* DefendingCharacterOwner = OtherComp->GetOwner();
			ACHAR_Player* DefendingCharacter = Cast<ACHAR_Player>(DefendingCharacterOwner);
			if (DefendingCharacter)
			{

				UGameplayStatics::ApplyDamage(DefendingCharacter, BulletProjectileDamage, AttackingPlayerController, AttackingCharacter, UDamageType::StaticClass());
			}
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
		
	else
	{
		FString hitNothing = FString::Printf(TEXT("ACTOR_BaseWeaponProjectile::Client Side Hit"));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, hitNothing);
		Destroy();
	}
}

#pragma endregion


#pragma region Networking

bool AACTOR_BaseWeaponProjectile::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const
{
	//Purpose: Ensures that the projectile is not replicated for the actor who fired it.  Otherwise, two projectiles would be shown on their screen. 
	AController* InstigatorController = GetInstigatorController();

	//UE_LOG(LogTemp, Warning, TEXT("Instigator: %s"), *InstigatorController->GetName());
	//UE_LOG(LogTemp, Warning, TEXT("RealViewer: %s"), *RealViewer->GetName());
	if (InstigatorController == RealViewer)
	{
			return false;
	}
	else
	{
		return Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);
	}
}
#pragma endregion




// I don't think that the projectile needs to evaluate tick functions
//PrimaryActorTick.bCanEverTick = true;