// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Asteroid.generated.h"


UCLASS()
class ASTEROIDSGAME_API AAsteroid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAsteroid();

	// Destructor which notifies the AsteroidField of destruction;
	~AAsteroid();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Function to handle the projectile hitting something
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector
			NormalImpulse, const FHitResult& Hit);
	// Direction vector
	UPROPERTY(EditAnywhere)
	FVector movementDirection;
	class AsteroidField* parent;
	void SetParent(AsteroidField* parent);

private:
	// A static mesh component. The visual representation of our actor
	class UStaticMeshComponent* AsteroidVisual;
	// A class to store the human player
	class APawn* Player;
	class WorldBoundaries* movementManager;	
};

