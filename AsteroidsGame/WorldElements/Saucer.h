// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "AwardsScoreInterface.h"
#include "Saucer.generated.h"

UCLASS()
class ASTEROIDSGAME_API ASaucer : public AActor, public IAwardsScoreInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASaucer();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual int AwardScore() const;
	
	void SetMovementDirection(FVector direction);

	FVector GetMovementDirection();

	// Function to handle the saucer hitting something
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector
			NormalImpulse, const FHitResult& Hit);
protected:
	// Reference to the player pawn, need to get it's current position
	APawn* PlayerPawn;

	/**
	*	Fires a projectile
	*/
	void Fire();

	bool CanFire;

	float GunOffset;

	float FireRate;

	class USoundBase* FireSound;

	// Handle for efficient management of ShotTimerExpired timer
	FTimerHandle TimerHandle_ShotTimerExpired;

	void SetCanFire();

	// Handle for the Change Rotation function
	FTimerHandle TimerHandle_ChangeRotation;

	void ChangeRotation();


	UPROPERTY(EditAnywhere)
	FVector MovementDirection;

	// The mesh component
	class UStaticMeshComponent* SaucerMeshComponent;

	class WorldBoundaries* movementManager;

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
