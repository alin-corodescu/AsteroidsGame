// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SpaceshipProjectile.generated.h"
/**
*	Class representing the projectiles used by spaceships and saucers
*/
UCLASS()
class ASTEROIDSGAME_API ASpaceshipProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	//! Sets default values for this actor's properties
	ASpaceshipProjectile();

	//! Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//! Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//! Function to handle the projectile hitting something
	/** 
	* Handles the logic of awarding points to the player if the projectile
	* is player owned
	* @see markAsPlayerOwned()
	*/
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			FVector NormalImpulse, const FHitResult& Hit);

	//! Marks the projectile as player-owned. Used for score-awarding logic
	void markAsPlayerOwned();



private:
	//! Sphere collision component
	UStaticMeshComponent* ProjectileMesh;
	//! Projectile movement component
	UProjectileMovementComponent* ProjectileMovement;

	//! Whether or not this projectile was spawned by the player pawn
	bool bIsPlayerOwned;
	
	//! Used to constrain the movements of the actor within the world boundaries.
	class WorldBoundaries* movementManager;
};
