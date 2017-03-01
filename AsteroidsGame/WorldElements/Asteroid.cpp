// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidsGame.h"
#include "Asteroid.h"
#include "WorldBoundaries.h"

// Sets default values
// Sets default values
AAsteroid::AAsteroid()
{
	PrimaryActorTick.bCanEverTick = true;
	// Create and attach a static mesh component.
	AsteroidVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AsteroidMesh"));
	// Set the mesh to the root component.
	RootComponent = AsteroidVisual;
	// Set the mesh for the static mesh component.
	ConstructorHelpers::FObjectFinder<UStaticMesh>
		AsteroidAsset(TEXT("StaticMesh'/Game/ExampleContent/Landscapes/Meshes/SM_Rock.SM_Rock'"));
	// If the mesh was found set it and set properties.
	if (AsteroidAsset.Succeeded())
	{
		AsteroidVisual->SetStaticMesh(AsteroidAsset.Object);
		//AsteroidVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -100.0f));
	}
	// set up a notification for when this component hits something
	AsteroidVisual->OnComponentHit.AddDynamic(this, &AAsteroid::OnHit);

	movementManager = WorldBoundaries::GetInstance();
}

// Called when the game starts or when spawned
void AAsteroid::BeginPlay()
{
	Super::BeginPlay();
	Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

// Called every frame
void AAsteroid::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	this->SetActorLocation(this->GetActorLocation() + movementDirection * DeltaTime);
	movementManager->CorrectPosition(this);
}


void AAsteroid::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent*
	OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Debug some info to the screen if needed
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Asteroid Hit! -> ") +
			OtherActor->GetName());
	}
	// Another better approach:
		if (this->IsA(OtherActor->GetActorClass()))
		{
			// Ignore collisions with other actors that whose name includes "asteroids"
			AsteroidVisual->MoveIgnoreActors.Add(OtherActor);
			// A debug message
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Asteroid Hit! Ignore collision.") );
			}
			return;
		}
	// First, check found the player pawn
	//if (Player)
	//{
	//	if (OtherActor->GetUniqueID() == Player->GetUniqueID())
		//{
			// Temp rotation.
			FRotator SpawnRotation = this->GetActorRotation();
			// Temp spawn location.
			FVector SpawnLocation = GetActorLocation();
			// Temp spawn location
			FVector ActorScale = this->GetActorScale3D();
			ActorScale.X = ActorScale.X * 0.8f;
			ActorScale.Y = ActorScale.Y * 0.8f;
			ActorScale.Z = ActorScale.Z * 0.8f;
			if (ActorScale.X > 0.3f)
			{
				UWorld* const World = GetWorld();
				if (World != NULL)
				{
					// Update the spawn location for the smaller rock.
					SpawnLocation.X = SpawnLocation.X + 20;
					// 1. Spawn a new asteroid
					AAsteroid* NewActor = World->SpawnActor<AAsteroid>(SpawnLocation,
						SpawnRotation);
					NewActor->SetActorScale3D(ActorScale);
					// Generate random number for movement
					float moveDirection = FMath::FRandRange(2.0f, 8.0);
					NewActor->movementDirection.X = moveDirection;
					moveDirection = FMath::FRandRange(2.0f, 8.0);
					// Set the movement direction of new actor
					NewActor->movementDirection.Y = moveDirection;
					// Update the spawn location for the smaller rock
					SpawnLocation.X = SpawnLocation.X - 40;
					// 2. Spawn a new asteroid
					NewActor = World->SpawnActor<AAsteroid>(SpawnLocation, SpawnRotation);
					NewActor->SetActorScale3D(ActorScale);
					moveDirection = -FMath::FRandRange(2.0f, 8.0);
					NewActor->movementDirection.X = moveDirection;
					moveDirection = -FMath::FRandRange(2.0f, 8.0);
					NewActor->movementDirection.Y = moveDirection;
				}
			}
			// Safely destroy this object
			Destroy();
		//}
	//}
}
