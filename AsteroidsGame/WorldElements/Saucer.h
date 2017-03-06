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

	int AwardScore() const override;
	
	void SetMovementDirection(FVector direction);

	FVector GetMovementDirection();

	// Function to handle the saucer hitting something
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector
			NormalImpulse, const FHitResult& Hit);
private:
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

	UPROPERTY(EditAnywhere)
	FVector MovementDirection;

	// The mesh component
	class UStaticMeshComponent* SaucerMeshComponent;

	class WorldBoundaries* movementManager;
};
