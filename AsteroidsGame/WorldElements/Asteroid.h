// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AwardsScoreInterface.h"
#include "AsteroidField.h"
#include "GameFramework/Actor.h"
#include "Asteroid.generated.h"
/**
*	Class representing asteroid actors.
*/
UCLASS()
class ASTEROIDSGAME_API AAsteroid : public AActor, public  IAwardsScoreInterface
{
	GENERATED_BODY()
	
public:	
	//! Sets default values for this actor's properties
	AAsteroid();

	//! Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//! Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//! Function to handle the projectile hitting something
	/*! Handles the logic of ignoring collisions with other asteroids*/
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector
			NormalImpulse, const FHitResult& Hit);

	//! Direction vector
	UPROPERTY(EditAnywhere)
	FVector movementDirection;
	
	//! Pointer to the AsteroidField which spawned this object
	class AsteroidField* parent;

	//! Sets the pointer to the AsteroidField which spawned the object
	void SetParent(AsteroidField* parent);

	//! Inherited from IAwardsScoreInterface
	int AwardScore() const override;

	/* @see Types */
	void SetType(Types type); //!< Setter for the type of the asteroid

	/* @see Types */
	Types GetType(); //!< Getter for the type of the asteroid

private:
	//! A static mesh component. The visual representation of our actor
	class UStaticMeshComponent* AsteroidVisual;

	//! Class used to constrain the movements of the actor within the boundaries
	class WorldBoundaries* movementManager;

	//! Type of the asteroid
	/* @see Types */
	Types type;
};

