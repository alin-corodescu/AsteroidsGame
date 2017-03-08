// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "SpaceshipPawn.generated.h"
/**
*	Class representing the Pawn controlled by the player during gameplay.
*/
UCLASS()
class ASTEROIDSGAME_API ASpaceshipPawn : public APawn
{
	GENERATED_BODY()

public:
	//! Sets default values for this pawn's properties
	ASpaceshipPawn();
	//! Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//! Called every frame
	/*! Produces an flickering effect on the visual component if invulnerable */
	virtual void Tick(float DeltaSeconds) override;
	//! Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	//! Move forward callback
	void MoveForwardInput( float Val);
	//! Move right callback
	void MoveRightInput(float Val);
	//! Fire forward callback
	void FireForwardInput(float Val);
	//! Hyperspace key callback
	void HyperspaceInput();

	//! Function to handle the spaceship hitting something
	/**
	* Defines the behaviour of the ship when colliding with something
	* reducing it's number of lives and relocating it to the origin, in an invulnerable state
	* Sets a timer to call RemoveInvulnerable() after 5 seconds
	* @see MakeInvulnerable()
	* @see RemoveInvulnerable();
	*/
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector
			NormalImpulse, const FHitResult& Hit);

	//! Puts the pawn in an invulnerable state, disabling collision.
	void MakeInvulnerable();
	//! Reverts the ship back to normal state, enabling collision.
	void RemoveInvulnerable();
	//! @return whether the Pawn is invulnerable or not
	/** @see MakeInvulnerable()
	* @see RemoveInvulnerable();
	*/
	bool IsInvulnerable();
private:
	//! The mesh component
	class UStaticMeshComponent* ShipMeshComponent;
	//! Current forward speed
	float CurrentForwardSpeed;
	//! Current rotation speed
	float CurrentRotationSpeed;
	//! Current fire value
	float CurrentFireValue;
	//! Flag to fire
	bool CanFire;
	//! How fast the weapon will fire
	float FireRate;
	//! Offset from the ships location to spawn projectiles
	float GunOffset;
	//! Handle for efficient management of ShotTimerExpired timer
	FTimerHandle TimerHandle_ShotTimerExpired;
	//! Sound to play each time we fire
	class USoundBase* FireSound;
	//! Sets shot flag to true. Used by a timer event
	void ShotTimerExpired();

	UPROPERTY(EditAnywhere)
	//! Maximum speed of the spaceship
	float MaxSpeed;

	//! Used to keep the ship inside the boundaries of the world
	class WorldBoundaries* movementManager;
	
	//! Flag for the invulnerable state
	bool bIsInvulnerable;
};
