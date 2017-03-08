// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidsGame.h"
#include "AsteroidField.h"
#include "WorldBoundaries.h"
#include "Asteroid.h"
#include "Spawner.h"

// Set the defualt values for scale and speed of large asteroids
float AsteroidField::DefaultScale = 9.0f;
float AsteroidField::SpeedScale = 100.0f;

AAsteroid * AsteroidField::ConstructAsteroid(FVector SpawnLocation, FRotator SpawnRotation, Types type)
{

	// Spawn a new asteroid
	AAsteroid* NewActor = world->SpawnActor<AAsteroid>(SpawnLocation, SpawnRotation);

	// Do the necessary init
	NewActor->SetParent(this);
	NewActor->SetType(type);

	// Set up a movement direction for a default (Large) asteroid
	float moveDirection = FMath::FRandRange(1.0f, 3.0f) * SpeedScale;
	NewActor->movementDirection.X = moveDirection;
	moveDirection = FMath::FRandRange(1.0f, 3.0f) * SpeedScale;
	NewActor->movementDirection.Y = moveDirection;

	FVector scale(DefaultScale, DefaultScale, DefaultScale);
	switch (type)
	{
		// Do the adjustments specific for each type
		case Large: 
			NewActor->SetActorScale3D(scale); 
			break;
		case Medium: 
			NewActor->SetActorScale3D(scale * 0.66f); 
			NewActor->movementDirection *= 1.2; 
			break;
		case Small: 
			NewActor->SetActorScale3D(scale * 0.33f); 
			NewActor->movementDirection *= 1.5;
			break;
	}

	// Insert the new actor in the map
	activeAsteroids.insert(std::make_pair(NewActor, type));
	return NewActor;
}

void AsteroidField::BreakUpAsteroid(AAsteroid * asteroid)
{
	// Get the type of the asteroid that need to be broken
	Types type = activeAsteroids.find(asteroid)->second;

	// Set up inital rotation and spawn location
	FVector spawnLocation = asteroid->GetActorLocation();
	FRotator initialRotation = asteroid->GetActorRotation();

	// Generate to different random rotations for the 2 new asteroids
	FRotator positiveRot(0,FMath::RandRange(10.0f, 30.0f) ,0);
	positiveRot += initialRotation;
	FRotator negativeRot(0, FMath::RandRange(-30.0f, -10.0f) ,0);
	negativeRot += initialRotation;

	switch (type)
	{
		// Add offsets to the spawn location for visual effect
	case Large:
		spawnLocation.X += 100;
		ConstructAsteroid(spawnLocation, positiveRot, Medium);
		spawnLocation.X -= 200;
		ConstructAsteroid(spawnLocation, negativeRot, Medium);
		break;
	case Medium:
		spawnLocation.X += 100;
		ConstructAsteroid(spawnLocation, positiveRot, Small);
		spawnLocation.X -= 200;
		ConstructAsteroid(spawnLocation, negativeRot, Small);
		break;
	default: /* To be completed */ break;
	}

}

AsteroidField::AsteroidField(UWorld* world)
{
	worldEdges = WorldBoundaries::GetInstance();
	this->world = world;
}

void AsteroidField::SpawnAsteroids(int Count)
{
	int i;
	FVector SpawnLocation(0,0,0);
	FRotator rotation(0,0,0);
	for (i = 0; i < Count; i++)
	{
		// Spawn asteroids on the edges of the world, one on each edge, circulary
		switch (i % 4)
		{
		case 0: 
			SpawnLocation.Y = worldEdges->Top; 
			SpawnLocation.X = FMath::FRandRange(worldEdges->Left,worldEdges->Right);
			SpawnLocation.Z = 0;
			break;
		case 1:
			SpawnLocation.X = worldEdges->Right;
			SpawnLocation.Y = FMath::FRandRange(worldEdges->Bottom, worldEdges->Top);
			SpawnLocation.Z = 0;
			break;
		case 2:
			SpawnLocation.Y = worldEdges->Bottom;
			SpawnLocation.X = FMath::FRandRange(worldEdges->Left, worldEdges->Right);
			SpawnLocation.Z = 0;
			break;
		case 3:
			SpawnLocation.X = worldEdges->Left;
			SpawnLocation.Y = FMath::FRandRange(worldEdges->Bottom, worldEdges->Top);
			SpawnLocation.Z = 0;
			break;
		}
		// Give the asteroid a random rotation
		rotation.Yaw = FMath::RandRange(-90.0f, 90.0f);

		// Spawn it in the world
		ConstructAsteroid(SpawnLocation, rotation, Large);
	}
}

void AsteroidField::NotifyDestruction(AAsteroid * asteroid)
{
	// Find the asteroid that has beed destroyed
	std::map<AAsteroid*, Types>::iterator it = activeAsteroids.find(asteroid);
	Types type = it->second;
	
	// Break it up if it's not a small one
	// It's safe because the destruction occurs at the end of the tick, not right away
	if (type != Small)
		BreakUpAsteroid(asteroid);

	// Erase it from the map
	if (it != activeAsteroids.end())
		activeAsteroids.erase(it);

	// Callback to the spawner if there are no asteroids left
	if (activeAsteroids.empty())
		spawner->NextSetOfAsteroids();
}

void AsteroidField::addSpawner(ASpawner * spawner)
{
	this->spawner = spawner;
}