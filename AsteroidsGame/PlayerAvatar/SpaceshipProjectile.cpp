// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidsGame.h"
#include "SpaceshipProjectile.h"
#include "AsteroidsPlayerState.h"
#include "../WorldElements/WorldBoundaries.h"
#include "../WorldElements/AwardsScoreInterface.h"


// Sets default values
ASpaceshipProjectile::ASpaceshipProjectile()
{
	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	// Static reference to the mesh to use for the projectile
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		ProjectileMeshAsset(TEXT("StaticMesh'/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile'"
		));
	// Create mesh component for the projectile sphere
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh0"));
	// Here we insert a mesh into the mesh we created
	ProjectileMesh->SetStaticMesh(ProjectileMeshAsset.Object);
	// Set the root component to the mesh
	ProjectileMesh->SetupAttachment(RootComponent);
	// Collision profiles are defined in DefaultEngine.ini
	ProjectileMesh->BodyInstance.SetCollisionProfileName("Projectile");
	// Set the root component to the mesh
	RootComponent = ProjectileMesh;
	// Set up a callback for when this component hits something
	ProjectileMesh->OnComponentHit.AddDynamic(this, &ASpaceshipProjectile::OnHit);
	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement =
		CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
	ProjectileMovement->UpdatedComponent = ProjectileMesh;
	ProjectileMovement->InitialSpeed = 2300.f;
	ProjectileMovement->MaxSpeed = 2300.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f; // No gravity

	// Die after 0.5 sec if no collision
	InitialLifeSpan = 0.5f;

	// Set the scale of the projectile
	FVector scale(1.0f, 1.0f, 1.0f);
	this->SetActorScale3D(scale);

	// Get the singleton instance of the WorldBoundaries class
	movementManager = WorldBoundaries::GetInstance();
}

// Called when the game starts or when spawned
void ASpaceshipProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpaceshipProjectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	// Keep the actor within the world boundaries
	movementManager->CorrectPosition(this);
}

void ASpaceshipProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Do the scoring if it is player owned
	if (bIsPlayerOwned)
	{
		APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		AAsteroidsPlayerState* CurrentState = NULL;
		if (Player)
			CurrentState = Cast<AAsteroidsPlayerState>(Player->PlayerState);
		 
		// Check if the other actor should award score
		bool bIsImplemented = OtherActor->GetClass()->ImplementsInterface(UAwardsScoreInterface::StaticClass());
		if (bIsImplemented)
		{
			IAwardsScoreInterface* ScoringObject = Cast<IAwardsScoreInterface>(OtherActor);
			// Add the score value to the current player score
			if (CurrentState)
				CurrentState->modifyScore(ScoringObject->AwardScore());
		}
	}

	// Destory this object
	Destroy();
}

void ASpaceshipProjectile::markAsPlayerOwned()
{
	this->bIsPlayerOwned = true;
}
