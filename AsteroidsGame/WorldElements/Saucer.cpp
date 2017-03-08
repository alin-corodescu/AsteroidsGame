// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidsGame.h"
#include "../PlayerAvatar/SpaceshipProjectile.h"
#include "WorldBoundaries.h"
#include "Saucer.h"


// Sets default values
ASaucer::ASaucer()
{
	//UE_LOG(LogTemp, Warning, TEXT("Saucer constructor called called Called"));
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GunOffset = 100.0f;
	FireRate = 2.0f;
	//actually wait 2 seconds before firing
	CanFire = false;

	SaucerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SaucerMesh"));
	RootComponent = SaucerMeshComponent;
	SaucerMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	SaucerMeshComponent->OnComponentHit.AddDynamic(this, &ASaucer::OnHit);
	// Get the ship mesh.
	// change it to the Saucer asset
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SaucerMesh(TEXT("StaticMesh'/Game/ExampleContent/Input_Examples/Meshes/SM_UFO_Main.SM_UFO_Main'"));
	// If the mesh was found set it and set properties.
	if (SaucerMesh.Succeeded())
		SaucerMeshComponent->SetStaticMesh(SaucerMesh.Object);
	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase>
		FireAudio(TEXT("SoundWave'/Game/TwinStick/Audio/TwinStickFire.TwinStickFire'"));
	if (FireAudio.Succeeded())
	{
		FireSound = FireAudio.Object;
	}
	// set a LifeSpan of 10.0s
	//this->InitialLifeSpan = 10.0f;
	movementManager = WorldBoundaries::GetInstance();

}

// Called when the game starts or when spawned
void ASaucer::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("Saucer begin called"));
	Super::BeginPlay();
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	FTimerHandle TimerHandle_StartOffset;
	// Wait 2 seconds before firing the first time
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_StartOffset, this, &ASaucer::SetCanFire, 2.0f);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_ChangeRotation, this, &ASaucer::ChangeRotation, 5.0f);
	LastUsedRotation = GetActorRotation();
	MovementDirection = GenerateMovementDirection();
}

// Called every frame
void ASaucer::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (CanFire)
	{
		// Rotates the Saucer towards the player
		EnterTargetingState();

		Fire();

		ExitTargetingState();
	}

	FVector location = MovementDirection * DeltaTime;
	this->AddActorLocalOffset(location, true);
	movementManager->CorrectPosition(this);
	//handle the modification of the MovementDirection
}

int ASaucer::AwardScore() const
{
	return 0;
}

void ASaucer::SetMovementDirection(FVector direction)
{
	this->MovementDirection = direction;
}

FVector ASaucer::GetMovementDirection()
{
	return MovementDirection;
}

void ASaucer::OnHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	// Should be safe to use since it occurs at the end of the tick
	// Debug some info to the screen if needed
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Saucer Hit! -> ") + OtherActor->GetName());
	}
	this->Destroy();
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

void ASaucer::ChangeRotation()
{
	float YawDelta = FMath::RandRange(-30, 30);
	// Generate a random rotation
	FRotator rotation(0, YawDelta ,0);

	this->SetActorRotation(GetActorRotation() + rotation);

	this->LastUsedRotation = GetActorRotation();
	// Drop the previous timer
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ChangeRotation);
	// Set up another random timer
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_ChangeRotation,this,&ASaucer::ChangeRotation,FMath::RandRange(3,7));

}

FVector ASaucer::GenerateMovementDirection()
{
	UE_LOG(LogTemp, Warning, TEXT("Wrong generate called"));
	return FVector(100,0,0);
}

void ASaucer::EnterTargetingState()
{
	return;
}

void ASaucer::ExitTargetingState()
{
	return;
}
