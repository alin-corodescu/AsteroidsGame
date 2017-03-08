// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Spawner.generated.h"
/* Class could be templated */
UCLASS()
class ASTEROIDSGAME_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void NextSetOfAsteroids();

private:

	int AsteroidsCount;

	FTimerHandle SaucerSpawning_Timer;

	void SpawnSaucer();

	class AsteroidField* field;

	class WorldBoundaries* worldBoundaries;
	
};
