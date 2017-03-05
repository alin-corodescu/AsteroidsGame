// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidsGame.h"
#include "../PlayerAvatar/SpaceshipProjectile.h"
#include "WorldBoundaries.h"
#include "Saucer.h"


// Sets default values
ASaucer::ASaucer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Get the player pawn to find it's location


	GunOffset = 50.0f;
	FireRate = 2.0f;
	//actually wait 2 seconds before firing
	CanFire = false;

	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SaucerMesh"));
	RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);


	// Get the ship mesh.
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		ShipMesh(TEXT("StaticMesh'/Game/ExampleContent/Input_Examples/Meshes/SM_UFO_Main.SM_UFO_Main'"));
	// If the mesh was found set it and set properties.
	if (ShipMesh.Succeeded())
	{
		ShipMeshComponent->SetStaticMesh(ShipMesh.Object);
	}
	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase>
		FireAudio(TEXT("SoundWave'/Game/TwinStick/Audio/TwinStickFire.TwinStickFire'"));
	if (FireAudio.Succeeded())
	{
		FireSound = FireAudio.Object;
	}

	movementManager = WorldBoundaries::GetInstance();
}

// Called when the game starts or when spawned
void ASaucer::BeginPlay()
{
	Super::BeginPlay();
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	FTimerHandle TimerHandle_StartOffset;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_StartOffset, this, &ASaucer::SetCanFire, 2.0f);
}

// Called every frame
void ASaucer::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (CanFire)
	{
		// Rotates the Saucer towards the player
		FRotator newRotation = (-(this->GetActorLocation() - PlayerPawn->GetActorLocation())).Rotation();

		this->SetActorRelativeRotation(newRotation);
		this->Fire();
	}

	FVector location = MovementDirection * DeltaTime;
	this->AddActorLocalOffset(location);
	movementManager->CorrectPosition(this);
	//handle the modification of the MovementDirection
}

int ASaucer::AwardScore() const
{
	return 1000;
}

void ASaucer::SetMovementDirection(FVector direction)
{
	this->MovementDirection = direction;
}

FVector ASaucer::GetMovementDirection()
{
	return MovementDirection;
}

void ASaucer::Fire()
{
	// Temp spawn rotation		
	FRotator SpawnRotation = GetActorRotation();
	// Temp spawn location
	FVector SpawnLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	ForwardVector = ForwardVector * GunOffset;
	// Update spawn location to relect to offset
	SpawnLocation = SpawnLocation + ForwardVector;
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		// Spawn the projectile
		ASpaceshipProjectile* NewActor = World->SpawnActor<ASpaceshipProjectile>(SpawnLocation, SpawnRotation);
		// Try and play the sound
		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}
		CanFire = false;
		World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ASaucer::SetCanFire, FireRate);
	}
}
void ASaucer::SetCanFire()
{
	CanFire = true;
}
