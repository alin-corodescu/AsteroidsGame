// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidsGame.h"
#include "Asteroid.h"
#include "WorldBoundaries.h"
#include "AsteroidField.h"

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

	// Set up the callback for collision
	AsteroidVisual->OnComponentHit.AddDynamic(this, &AAsteroid::OnHit);
	
	// Get the singleton instance of the WorldBoundaries class
	movementManager = WorldBoundaries::GetInstance();

	// Set up destruction sound
	static ConstructorHelpers::FObjectFinder<USoundBase>
		DestructionAudio(TEXT("SoundWave'/Game/StarterContent/Audio/Explosion01.Explosion01'"));
	if (DestructionAudio.Succeeded())
	{
		DestructionSound = DestructionAudio.Object;
	}

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

	// Move the actor, with collision (sweep)
	FVector location = movementDirection * DeltaTime;
	this->AddActorLocalOffset(location, true);

	// Corret the actor's position, if necessary
	movementManager->CorrectPosition(this);
}


void AAsteroid::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent*
	OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// If collision occured with another asteroid, ignore it
	if (this->IsA(OtherActor->GetActorClass()))
	{
		AsteroidVisual->MoveIgnoreActors.Add(OtherActor);
		return;
	}
	UGameplayStatics::PlaySoundAtLocation(this, DestructionSound, GetActorLocation());
	// Callback for the AsteroidField upon destruction
	parent->NotifyDestruction(this);
	// We can destroy the asteroid
	Destroy();
}

void AAsteroid::SetParent(AsteroidField* parent)
{
	this->parent = parent;
}

int AAsteroid::AwardScore() const
{
	// Awards different scores based on type
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


