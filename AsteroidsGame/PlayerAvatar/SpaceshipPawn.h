// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "SpaceshipPawn.generated.h"

UCLASS()
class ASTEROIDSGAME_API ASpaceshipPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASpaceshipPawn();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// Move forward
	void MoveForwardInput(float Val);
	// Move right
	void MoveRightInput(float Val);
	// Fire forward
	void FireForwardInput(float Val);

	// Function to handle the projectile hitting something
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector
			NormalImpulse, const FHitResult& Hit);

	void MakeInvulnerable();

	void RemoveInvulnerable();
private:
	// The mesh component
	class UStaticMeshComponent* ShipMeshComponent;
	// Current forward speed
	float CurrentForwardSpeed;
	// Current rotation speed
	float CurrentRotationSpeed;
	// Current fire value
	float CurrentFireValue;
	// Flag to fire
	bool CanFire;
	// How fast the weapon will fire
	float FireRate;
	// Offset from the ships location to spawn projectiles
	float GunOffset;
	// Handle for efficient management of ShotTimerExpired timer
	FTimerHandle TimerHandle_ShotTimerExpired;
	// Sound to play each time we fire
	class USoundBase* FireSound;
	// Sets shot flag to true. Used by a timer event
	void ShotTimerExpired();

	UPROPERTY(EditAnywhere)
	float MaxSpeed;

	class WorldBoundaries* movementManager;
	
	bool bIsInvulnerable;
};
