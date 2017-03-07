// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include <map>
/**
 * 
 */
class AAsteroid;
enum Types { Large, Medium, Small };

class ASTEROIDSGAME_API AsteroidField
{
private:
	std::map<AAsteroid*,Types> activeAsteroids;
	class WorldBoundaries*  worldEdges;
	class UWorld* world;
	/* Scaling factor for a large (default) asteroid*/
	static float DefaultScale;
	/* Speed factor for a large (default) asteroid */
	static float SpeedScale;
	
	/* Constructs a new Asteroid of the specified type at the spawn location */
	AAsteroid* ConstructAsteroid(FVector SpawnLocation, FRotator SpawnRotation, Types type, bool NegativeMovement = false);

	/* Spawns the 2 smaller asteroids after this one is destroyed*/
	void BreakUpAsteroid(AAsteroid* asteroid);
	class ASpawner* spawner;
public:
	AsteroidField(UWorld* world);
	~AsteroidField();
	/**
	 *	Spawns a new set of large asteroids on the edges
	 *  of the viewport and gives them a random movement direction
	 *  @param Count number of asteroids to be spawned
	 */
	void SpawnAsteroids(int Count);

	/* Notifies the AsteroidField that an asteroid has been destroyed */
	void NotifyDestruction(AAsteroid* asteroid);

	void addSpawner(ASpawner * spawner);
};
