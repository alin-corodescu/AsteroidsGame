// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "AwardsScoreInterface.h"
#include "Saucer.generated.h"
/**
* Base class for Saucers. Includes common functionality and members.
* Should not be spawned in the world.
* (can't make it abstract due to UE restrictions)
* @see LargeSaucer
* @see SmallSaucer
*/
UCLASS()
class ASTEROIDSGAME_API ASaucer : public AActor, public IAwardsScoreInterface
{
	GENERATED_BODY()
	
public:	
	//! Sets default values for this actor's properties
	ASaucer();

	//! Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//! Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//! Inherited from the IAwardsScoreInterface
	virtual int AwardScore() const;
	
	//! Sets the movement direction vector of this saucer
	void SetMovementDirection(FVector direction);

	//! @returns the movement direction of this saucer
	FVector GetMovementDirection();

	// Function to handle the saucer hitting something
	/*! Simply destorys the object */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector
			NormalImpulse, const FHitResult& Hit);
protected:
	/** Reference to the player pawn.
	* Needed to get it's current position
	*/
	APawn* PlayerPawn;
	//! Fires a projectile
	void Fire();

	//! Flag whether or not the saucer can fire
	bool CanFire;
	
	//! The offset at which to spawn the projectile so it doesn't collide with the saucer
	float GunOffset;

	//! How often the ship can fire
	float FireRate;

	//! Sound to play when firing
	class USoundBase* FireSound;

	// Handle for efficient management of ShotTimerExpired timer
	FTimerHandle TimerHandle_ShotTimerExpired;

	//! Callback for the timer managing the Fire rate
	void SetCanFire();

	//! Handle for the Change Rotation function
	FTimerHandle TimerHandle_ChangeRotation;

	//! Changes rotation randomly
	/*! Used to make it harder to hit */
	void ChangeRotation();

	//! Movement Direction of the saucer
	UPROPERTY(EditAnywhere)
	FVector MovementDirection;

	//! The mesh component
	class UStaticMeshComponent* SaucerMeshComponent;

	//! Used to constrain the movement of the Saucer within the world boundaries
	class WorldBoundaries* movementManager;

	//! Sound to play when destroyed
	class USoundBase* DestructionSound;

	/**
	* Member used to store the rotation of the saucer.
	* Used when exiting targeting state to restore the saucer to it's previous rotation.
	* @see EnterTargetingState
	* @see ExitTargetingState
	*/
	FRotator LastUsedRotation;

	// Generates a Movement Direction vector specific to the type of the object
	virtual FVector GenerateMovementDirection();

	/**
	*	Modifies the actor rotation using it's targeting logic
	*	To be used in conjuction with ExitTargetingState()
	*	/sa ExitTargetingState
	*/
	virtual void EnterTargetingState();
	/**
	*	Reverts the actor rotation to the one used before calling
	*	EnterTargetingState(), has no effect otherwise.
	*/
	virtual void ExitTargetingState();

};
