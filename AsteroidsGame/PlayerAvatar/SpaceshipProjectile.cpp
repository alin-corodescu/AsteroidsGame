// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidsGame.h"
#include "SpaceshipProjectile.h"
#include "AsteroidsPlayerState.h"
#include "../WorldElements/AwardsScoreInterface.h"


// Sets default values
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
	// set up a notification for when this component hits something
	ProjectileMesh->OnComponentHit.AddDynamic(this, &ASpaceshipProjectile::OnHit);
	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement =
		CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
	ProjectileMovement->UpdatedComponent = ProjectileMesh;
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f; // No gravity
													  // Die after 3 seconds by default. This is a property of AActor
	InitialLifeSpan = 3.0f;
	// Set the scale of the projectile
	FVector scale(1.0f, 1.0f, 1.0f);
	this->SetActorScale3D(scale);
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

}

void ASpaceshipProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Debug some info to the screen if needed
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Projectile Hit! -> ") + OtherActor ->GetName());
	}
	// Safely destroy this object
	//here i need to update the score
	if (bIsPlayerOwned)
	{
		APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		AAsteroidsPlayerState* CurrentState = NULL;
		if (Player)
			CurrentState = Cast<AAsteroidsPlayerState>(Player->PlayerState);
		//move when it sets bIsPlayerOwned;
		 
		bool bIsImplemented = OtherActor->GetClass()->ImplementsInterface(UAwardsScoreInterface::StaticClass());
		if (bIsImplemented)
		{
			IAwardsScoreInterface* ScoringObject = Cast<IAwardsScoreInterface>(OtherActor);
			if (CurrentState)
				CurrentState->modifyScore(ScoringObject->AwardScore());
		}
	}
	Destroy();
}

void ASpaceshipProjectile::markAsPlayerOwned()
{
	this->bIsPlayerOwned = true;
}
