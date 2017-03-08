// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * Class used to constrain the movement of Actors 
 * within the world boundaries
 * -- could be turned into an abstract interface for flexibility --
 */
class ASTEROIDSGAME_API WorldBoundaries
{
	friend class AsteroidField;
	friend class ASpawner;
	friend class ASpaceshipPawn;
private:
	WorldBoundaries();
	~WorldBoundaries();
	// Pointer to the single instance of this class
	static WorldBoundaries* instance;
	// Coordinates of the world boundaries
	float Right, Left, Top, Bottom;
public:
	// Static function used to access an instance
	static WorldBoundaries* GetInstance();

	/* Computes World Boundaries based on a orthographic static camera */
	void SetUpLimits(const UCameraComponent* camera);

	bool CorrectPosition(AActor* actor);

};
