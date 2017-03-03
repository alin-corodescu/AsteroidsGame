// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidsGame.h"
#include "AsteroidField.h"
#include "WorldBoundaries.h"
#include "Asteroid.h"

float AsteroidField::DefaultScale = 9.0f;
float AsteroidField::SpeedScale = 100.0f;

AAsteroid * AsteroidField::ConstructAsteroid(FVector SpawnLocation, FRotator SpawnRotation, Types type, bool NegativeMovement)
{
	UE_LOG(LogTemp, Warning, TEXT("ConstructAsteroid Called"));
	FVector scale(DefaultScale, DefaultScale, DefaultScale);
	AAsteroid* NewActor = world->SpawnActor<AAsteroid>(SpawnLocation, SpawnRotation);
	NewActor->SetParent(this);
	switch (type)
	{
		case Large: NewActor->SetActorScale3D(scale); break;
		case Medium: NewActor->SetActorScale3D(scale * 0.66f); break;
		case Small: NewActor->SetActorScale3D(scale * 0.33f); break;
	}
	// Generate random number for movement
	float moveDirection = FMath::FRandRange(1.0f, 3.0f) * SpeedScale;
	NewActor->movementDirection.X = moveDirection;
	moveDirection = FMath::FRandRange(1.0f, 3.0f) * SpeedScale;
	// Set the movement direction of new actor
	NewActor->movementDirection.Y = moveDirection;

	activeAsteroids.insert(std::make_pair(NewActor, type));
	return NewActor;
}

void AsteroidField::BreakUpAsteroid(AAsteroid * asteroid)
{
	// Get the type of the asteroid that need to be broken
	UE_LOG(LogTemp, Warning, TEXT("Breaking function called Called"));
	Types type = activeAsteroids.find(asteroid)->second;
	FVector spawnLocation = asteroid->GetActorLocation();
	switch (type)
	{
	case Large:
		spawnLocation.X += 100;
		ConstructAsteroid(spawnLocation, FRotator(0, 0, 0), Medium, false);
		spawnLocation.X -= 200;
		ConstructAsteroid(spawnLocation, FRotator(0, 0, 0), Medium, true);
		break;
	case Medium:
		spawnLocation.X += 100;
		ConstructAsteroid(spawnLocation, FRotator(0, 0, 0), Small, false);
		spawnLocation.X -= 200;
		ConstructAsteroid(spawnLocation, FRotator(0, 0, 0), Small, true);
		break;
	default: /* To be completed */ break;
	}

}

AsteroidField::AsteroidField(UWorld* world)
{
	worldEdges = WorldBoundaries::GetInstance();
	this->world = world;
}

AsteroidField::~AsteroidField()
{
}

void AsteroidField::SpawnAsteroids(int Count)
{
	int i;
	FVector SpawnLocation;
	FRotator rotation(0,0,0);
	for (i = 0; i < Count; i++)
	{
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

		ConstructAsteroid(SpawnLocation, rotation, Large, FMath::RandBool());
	}
}

void AsteroidField::NotifyDestruction(AAsteroid * asteroid)
{
	UE_LOG(LogTemp, Warning, TEXT("Notify destruction Called"));
	std::map<AAsteroid*, Types>::iterator it = activeAsteroids.find(asteroid);
	Types type = it->second;
	if (type != Small)
		BreakUpAsteroid(asteroid);
	if (it != activeAsteroids.end())
	{
		activeAsteroids.erase(it);
	}
}
