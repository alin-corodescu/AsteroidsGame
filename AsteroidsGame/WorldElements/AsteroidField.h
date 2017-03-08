// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include <map>
/**
 * Class which handles the logic of spawninng and breaking up asteroids.
 */
class AAsteroid;
enum Types { Large, Medium, Small };

class ASTEROIDSGAME_API AsteroidField
{
private:
	//! Map containing pointers to the spawned asteroids.
	/*! Could be a std::set really, but refactoring takes precious time */
	std::map<AAsteroid*,Types> activeAsteroids;

	//! Class used to get the get the coordinates of the world edges
	/*! Used to determine spawning locations of the asteroids */
	class WorldBoundaries*  worldEdges;

	//! Reference to the world in which to spawn asteroids.
	class UWorld* world;

	/*! Scaling factor for a large (default) asteroid*/
	static float DefaultScale;

	/*! Speed factor for a large (default) asteroid */
	static float SpeedScale;
	
	/*! Constructs a new Asteroid of the specified type at the spawn location */
	AAsteroid* ConstructAsteroid(FVector SpawnLocation, FRotator SpawnRotation, Types type);

	/*! Spawns the 2 smaller asteroids after this one is destroyed*/
	void BreakUpAsteroid(AAsteroid* asteroid);

	//! Pointer to the Spawner.
	/*! Used to notify when there are no asteroids left in the world */
	class ASpawner* spawner;
public:
	//! Constructor with world as argument
	AsteroidField(UWorld* world);

	/**
	 *	Spawns a new set of large asteroids on the edges
	 *  of the viewport and gives them a random movement direction
	 *  @param Count number of asteroids to be spawned
	 */
	void SpawnAsteroids(int Count);

	/* Notifies the AsteroidField that an asteroid has been destroyed */
	//! Removes the asteroid from the active asteroids map
	/*! @see activeAsteroids */
	void NotifyDestruction(AAsteroid* asteroid);

	//! Sets the Spawner
	/*! @see spawner */
	void addSpawner(ASpawner * spawner);
};
