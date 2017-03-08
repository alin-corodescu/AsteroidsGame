// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidsGame.h"
#include "AsteroidField.h"
#include "Spawner.h"
#include "../PlayerAvatar/AsteroidsPlayerState.h"
#include "SmallSaucer.h"
#include "LargeSaucer.h"
#include "WorldBoundaries.h"


#define SMALL_SAUCERS_ONLY_THRESHOLD 10000

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Initial level - 4 asteroids
	AsteroidsCount = 4;

}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	// Create a new Asteroid Field
	AsteroidField* field = new AsteroidField(GetWorld());
	this->field = field;
	field->addSpawner(this);

	// Spawn the first set of asteroids
	field->SpawnAsteroids(AsteroidsCount);

	// Set up a random timer for spawning saucers
	GetWorld()->GetTimerManager().SetTimer(SaucerSpawning_Timer, this, &ASpawner::SpawnSaucer, FMath::RandRange(20, 30));

	// Get the instance of this singleton
	worldBoundaries = WorldBoundaries::GetInstance();
}

// Called every frame
void ASpawner::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ASpawner::NextSetOfAsteroids()
{
	// Impose a limit of asteroids
	if (AsteroidsCount < 10)
		AsteroidsCount++;

	field->SpawnAsteroids(AsteroidsCount);
}

void ASpawner::SpawnSaucer()
{
	// Create a SpawnLocation and Rotation
	FVector SpawnLocation;
	FRotator SpawnRotation(0,0,0);
	SpawnLocation.X = worldBoundaries->Left;
	// Location can be on either left or right edges of the world
	SpawnLocation.Y = FMath::RandRange(worldBoundaries->Bottom, worldBoundaries->Top);
	SpawnLocation.Z = 0;
	// Decide whether it should be Left or Right
	// The rotation combined with the position correction will produce the desired effect
	if (FMath::RandBool() == false)
		SpawnRotation.Yaw = 180;

	// As a placeholder for another mesh just turn the spaceship upside down and create a saucer!
	SpawnRotation.Roll = 180;


	UWorld* world = GetWorld();

	// Determine the spawning type (Large or Small) depending on the current score
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(world, 0);
	AAsteroidsPlayerState* CurrentState = Cast<AAsteroidsPlayerState>(PlayerPawn->PlayerState);
	int Score = CurrentState->GetScore();

	// Give a chance when the score is low to the large saucers
	float Chance = FMath::FRand();

	// The higher the score gets, the less the chance of spawning a LargeSaucer, up to a threshold
	// when only small saucers will be spawned
	if (Chance < (float) Score / SMALL_SAUCERS_ONLY_THRESHOLD)
		ASmallSaucer* NewActor = world->SpawnActor<ASmallSaucer>(SpawnLocation, SpawnRotation);
	else 
		ALargeSaucer* NewActor = world->SpawnActor<ALargeSaucer>(SpawnLocation, SpawnRotation);

	// Reset the timer for spawning another saucer
	world->GetTimerManager().ClearTimer(SaucerSpawning_Timer);
	world->GetTimerManager().SetTimer(SaucerSpawning_Timer, this, &ASpawner::SpawnSaucer, FMath::RandRange(20, 40));
}

