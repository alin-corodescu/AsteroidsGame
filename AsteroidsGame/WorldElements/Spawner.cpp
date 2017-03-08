// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidsGame.h"
#include "AsteroidField.h"
#include "Spawner.h"
#include "../PlayerAvatar/AsteroidsPlayerState.h"
#include "SmallSaucer.h"
#include "LargeSaucer.h"
#include "WorldBoundaries.h"


#define SMALL_SAUCERS_ONLY_THRESHOLD 10000

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AsteroidsCount = 4;

}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	AsteroidField* field = new AsteroidField(GetWorld());
	this->field = field;
	field->addSpawner(this);
	field->SpawnAsteroids(AsteroidsCount);

	GetWorld()->GetTimerManager().SetTimer(SaucerSpawning_Timer, this, &ASpawner::SpawnSaucer, FMath::RandRange(20, 30));

	worldBoundaries = WorldBoundaries::GetInstance();
}

// Called every frame
void ASpawner::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ASpawner::NextSetOfAsteroids()
{
	if (AsteroidsCount < 10)
		AsteroidsCount++;

	field->SpawnAsteroids(AsteroidsCount);
}

void ASpawner::SpawnSaucer()
{
	UE_LOG(LogTemp, Warning, TEXT("Spawn called"));
	FVector SpawnLocation;
	FRotator SpawnRotation(0,0,0);
	SpawnLocation.X = worldBoundaries->Left;
	SpawnLocation.Y = FMath::RandRange(worldBoundaries->Bottom, worldBoundaries->Top);
	SpawnLocation.Z = 0;
	if (FMath::RandBool() == false)
		SpawnRotation.Yaw = 180;
	SpawnRotation.Roll = 180;


	UWorld* world = GetWorld();

	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(world, 0);
	AAsteroidsPlayerState* CurrentState = Cast<AAsteroidsPlayerState>(playerPawn->PlayerState);
	int score = CurrentState->GetScore();

	if (score < SMALL_SAUCERS_ONLY_THRESHOLD)
	{
		float chance = FMath::FRand();
		if (chance < (float) score / SMALL_SAUCERS_ONLY_THRESHOLD)
			ASmallSaucer* NewActor = world->SpawnActor<ASmallSaucer>(SpawnLocation, SpawnRotation);
		else 
			ALargeSaucer* NewActor = world->SpawnActor<ALargeSaucer>(SpawnLocation, SpawnRotation);
	}
	else
		ASmallSaucer* NewActor = world->SpawnActor<ASmallSaucer>(SpawnLocation, SpawnRotation);

	world->GetTimerManager().ClearTimer(SaucerSpawning_Timer);
	world->GetTimerManager().SetTimer(SaucerSpawning_Timer, this, &ASpawner::SpawnSaucer, FMath::RandRange(20, 40));
}

