// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidsGame.h"
#include "Asteroid.h"
#include "WorldBoundaries.h"
#include "AsteroidField.h"

// Sets default values
// Sets default values
AAsteroid::AAsteroid()
{
	PrimaryActorTick.bCanEverTick = true;
	// Create and attach a static mesh component.
	AsteroidVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AsteroidMesh"));
	
	// Set the mesh to the root component.
	RootComponent = AsteroidVisual;

	// Set the mesh for the static mesh component.
	ConstructorHelpers::FObjectFinder<UStaticMesh>
		AsteroidAsset(TEXT("StaticMesh'/Game/ExampleContent/Landscapes/Meshes/SM_Rock.SM_Rock'"));
	// If the mesh was found set it and set properties.
	if (AsteroidAsset.Succeeded())
	{
		AsteroidVisual->SetStaticMesh(AsteroidAsset.Object);
	}

	// set up a notification for when this component hits something
	AsteroidVisual->OnComponentHit.AddDynamic(this, &AAsteroid::OnHit);
	
	movementManager = WorldBoundaries::GetInstance();
	this->parent = parent;
}

// Called when the game starts or when spawned
void AAsteroid::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAsteroid::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	FVector location = movementDirection * DeltaTime;
	this->AddActorLocalOffset(location, true);
	//this->SetActorLocation(this->GetActorLocation() + movementDirection * DeltaTime,true);
	movementManager->CorrectPosition(this);
}


void AAsteroid::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent*
	OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Debug some info to the screen if needed
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Asteroid Hit! -> ") + OtherActor->GetName());
	}
	// Another better approach:
	if (this->IsA(OtherActor->GetActorClass()))
	{
		AsteroidVisual->MoveIgnoreActors.Add(OtherActor);
		// A debug message
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Asteroid Hit! Ignore collision.") );
		}
		return;
	}
	// We can destroy the asteroid
	parent->NotifyDestruction(this);
	Destroy();
}

void AAsteroid::SetParent(AsteroidField* parent)
{
	this->parent = parent;
}

int AAsteroid::AwardScore() const
{
	switch (type)
	{
	case Large: return 20;
	case Medium: return 50;
	case Small: return 100;
	}
	return 0;
}

void AAsteroid::SetType(Types type)
{
	this->type = type;
}

Types AAsteroid::GetType()
{
	return type;
}


