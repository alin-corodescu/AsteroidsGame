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
		//AsteroidVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -100.0f));
	}
	// set up a notification for when this component hits something
	AsteroidVisual->OnComponentHit.AddDynamic(this, &AAsteroid::OnHit);

	movementManager = WorldBoundaries::GetInstance();
	this->parent = parent;
}

AAsteroid::~AAsteroid()
{
	parent->NotifyDestruction(this);
}

// Called when the game starts or when spawned
void AAsteroid::BeginPlay()
{
	Super::BeginPlay();
	Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

// Called every frame
void AAsteroid::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	this->SetActorLocation(this->GetActorLocation() + movementDirection * DeltaTime);
	movementManager->CorrectPosition(this);
}


void AAsteroid::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent*
	OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Debug some info to the screen if needed
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Asteroid Hit! -> ") +
			OtherActor->GetName());
	}
	// Another better approach:
		if (this->IsA(OtherActor->GetActorClass()))
		{
			// Ignore collisions with other actors that whose name includes "asteroids"
			AsteroidVisual->MoveIgnoreActors.Add(OtherActor);
			// A debug message
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Asteroid Hit! Ignore collision.") );
			}
			return;
		}
		Destroy();
}

void AAsteroid::SetParent(AsteroidField* parent)
{
	this->parent = parent;
}