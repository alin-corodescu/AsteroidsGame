// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Spawner.generated.h"
/** 
* Class used to handle the spawning logic of the actors, excluding the player avatar.
*/
UCLASS()
class ASTEROIDSGAME_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	//! Sets default values for this actor's properties
	ASpawner();

	//! Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//! Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//! Callback functions used to spawn a new set of asteroids
	void NextSetOfAsteroids();

private:
	/** Counter used to know the number of asteroids to be spawned 
	* by the NextSetOfAsteroids callback.
	*/
	int AsteroidsCount;

	//! TimerHandle for Saucer spawning timer
	FTimerHandle SaucerSpawning_Timer;

	//! Function used to spawn a saucer
	void SpawnSaucer();

	//! Pointer to the AsteroidField to be used to spawn asteroids
	class AsteroidField* field;

	//! Class used to get the worldBoundaries, to be able to spawn Saucers.
	class WorldBoundaries* worldBoundaries;
	
};
