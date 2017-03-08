// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidsGame.h"
#include "SpaceshipPawn.h"
#include "SpaceshipProjectile.h"
#include "../WorldElements/WorldBoundaries.h"
#include "AsteroidsPlayerState.h"

// Sets default values
ASpaceshipPawn::ASpaceshipPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxSpeed = 1000.0f;
	// Create the mesh component.
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	// set up a notification for when this component hits something
	ShipMeshComponent->OnComponentHit.AddDynamic(this, &ASpaceshipPawn::OnHit);

	// Get the ship mesh.
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		ShipMesh(TEXT("StaticMesh'/Game/ExampleContent/Input_Examples/Meshes/SM_UFO_Main.SM_UFO_Main'"));
	// If the mesh was found set it and set properties.
	if (ShipMesh.Succeeded())
	{
		ShipMeshComponent->SetStaticMesh(ShipMesh.Object);
	}
	CurrentForwardSpeed = 0.0f;
	CurrentRotationSpeed = 0.0f;
	CurrentFireValue = 0.0f;
	// Set fire flag, fire rate and offset.
	CanFire = true;
	FireRate = 1.0f;
	GunOffset = 70.0f;
	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase>
		FireAudio(TEXT("SoundWave'/Game/TwinStick/Audio/TwinStickFire.TwinStickFire'"));
	if (FireAudio.Succeeded())
	{
		FireSound = FireAudio.Object;
	}

	movementManager = WorldBoundaries::GetInstance();

	bIsInvulnerable = false;
}

// Called when the game starts or when spawned
void ASpaceshipPawn::BeginPlay()
{
	Super::BeginPlay();

	// Get the player state
	AAsteroidsPlayerState* CurrentState = Cast<AAsteroidsPlayerState>(this->PlayerState);
	if (CurrentState)
	{
		CurrentState->SetScore(0);
		CurrentState->SetNumberOfLives(3);
	}
}

// Called every frame
void ASpaceshipPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	// best invulnerability animation EVER
	if (IsInvulnerable())
	{
		// Trigger visibility every 0.1 sec
		static float DisplayTimer = 0.1f;
		static bool invisible = true;
		DisplayTimer -= DeltaTime;
		if (DisplayTimer <= 0)
		{
			SetActorHiddenInGame(invisible);
			DisplayTimer = 0.1f;
			invisible = !invisible;
		}
	}
	else
		// Make sure the actor is showing when not invulnerable
		SetActorHiddenInGame(false);

	// Move Forward and back.
	const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaTime, 0.f, 0.f);
	// Move forwards (with sweep so we stop when we collide with things)
	AddActorLocalOffset(LocalMove, true);
	// Checks if we reached the end of the world
	movementManager->CorrectPosition(this);
	// Calculate change in rotation this frame
	FRotator DeltaRotation(0, 0, 0);
	DeltaRotation.Yaw = CurrentRotationSpeed * DeltaTime;
	// Rotate spaceship
	AddActorLocalRotation(DeltaRotation);

	// Consume fire input
	if (CurrentFireValue == 1.0f)
	{
		if (CanFire)
		{
			// Spawn rotation
			FRotator SpawnRotation = this->GetActorRotation();
			// Spawn location
			FVector SpawnLocation = GetActorLocation();
			FVector ForwardVector = GetActorForwardVector();
			ForwardVector = ForwardVector * GunOffset;
			// Update spawn location to reflect to offset
			SpawnLocation = SpawnLocation + ForwardVector;

			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				// Spawn the projectile
				ASpaceshipProjectile* NewActor = World->SpawnActor<ASpaceshipProjectile>(SpawnLocation, SpawnRotation);
				NewActor->markAsPlayerOwned();
				// Try and play the sound
				if (FireSound != nullptr)
				{
					UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
				}
				// Set up timer to update the CanFire flag
				World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this,
					&ASpaceshipPawn::ShotTimerExpired, FireRate);

				// Wait until we can fire again
				CanFire = false;
			}
		}
	}


}

// Called to bind functionality to input
void ASpaceshipPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	// Bind movement to callback functions.
	InputComponent->BindAction("Hyperspace", IE_Pressed, this, &ASpaceshipPawn::HyperspaceInput);
	InputComponent->BindAxis("MoveForward", this, &ASpaceshipPawn::MoveForwardInput);
	InputComponent->BindAxis("MoveRight", this, &ASpaceshipPawn::MoveRightInput);
	InputComponent->BindAxis("FireForward", this, &ASpaceshipPawn::FireForwardInput);

}

// Called when a rotate left or right event occurs.
void ASpaceshipPawn::MoveRightInput(float Val)
{
	// Is there no input?
	bool bHasInput = !FMath::IsNearlyEqual(Val, 0.f);
	if (bHasInput)
		CurrentRotationSpeed = Val * 100.0f;
	else
		CurrentRotationSpeed = 0;
}

// Called when a move forward or backward event occurs.
void ASpaceshipPawn::MoveForwardInput(float Val)
{
	// Is there no input?
	bool bHasInput = !FMath::IsNearlyEqual(Val, 0.f);
	if (bHasInput)
	{
		if (CurrentForwardSpeed < MaxSpeed)
			CurrentForwardSpeed += Val * 10.0f;
	}
	else
	{
		if (CurrentForwardSpeed > 10.0f)
			CurrentForwardSpeed /= 1.05f;
		else
			CurrentForwardSpeed = 0;
	}
}

// Called when fire is pressed.
void ASpaceshipPawn::FireForwardInput(float Val)
{
	CurrentFireValue = Val;
}
void ASpaceshipPawn::HyperspaceInput()
{
	FVector newLocation;

	// Generate a random Location
	newLocation.X = FMath::RandRange(movementManager->Left, movementManager->Right);
	newLocation.Y = FMath::RandRange(movementManager->Bottom, movementManager->Top);
	newLocation.Z = 0;

	SetActorLocation(newLocation);
}
void ASpaceshipPawn::OnHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	if (!bIsInvulnerable)
	{
		// Get the player state
		AAsteroidsPlayerState* CurrentState = Cast<AAsteroidsPlayerState>(this->PlayerState);
		// Reduce the number of lives;
		CurrentState->modifyLives(-1);

		// Make the spaceship invulnerable for 5 secs;
		this->MakeInvulnerable();
		FTimerHandle InvulnerabilityTimer;
		GetWorld()->GetTimerManager().SetTimer(InvulnerabilityTimer, this, &ASpaceshipPawn::RemoveInvulnerable, 5.0);

		// Reset the ship's position;
		this->SetActorLocation(FVector(0, 0, 0));
	}
}
void ASpaceshipPawn::MakeInvulnerable()
{
	bIsInvulnerable = true;
	SetActorEnableCollision(false);
}
void ASpaceshipPawn::RemoveInvulnerable()
{
	bIsInvulnerable = false;
	SetActorEnableCollision(true);
}
bool ASpaceshipPawn::IsInvulnerable()
{
	return bIsInvulnerable;
}
// Sets shot flag to true. Used by a timer event
void ASpaceshipPawn::ShotTimerExpired()
{
	CanFire = true;
}