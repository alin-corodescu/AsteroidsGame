// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidsGame.h"
#include "../PlayerAvatar/SpaceshipProjectile.h"
#include "WorldBoundaries.h"
#include "Saucer.h"
// Sets default values
ASaucer::ASaucer()
{
	PrimaryActorTick.bCanEverTick = true;

	// Offset at which to spawn the projectile
	GunOffset = 100.0f;

	// Fire once every 2 seconds
	FireRate = 2.0f;

	// Do not fire right away
	CanFire = false;

	// Set up the root component collision
	SaucerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SaucerMesh"));
	RootComponent = SaucerMeshComponent;
	SaucerMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	// Add the callback for collision
	SaucerMeshComponent->OnComponentHit.AddDynamic(this, &ASaucer::OnHit);

	// Get the ship mesh.
	// Should use a different mesh for saucers
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


	static ConstructorHelpers::FObjectFinder<USoundBase>
		DestructionAudio(TEXT("SoundWave'/Game/StarterContent/Audio/Explosion01.Explosion01'"));
	if (DestructionAudio.Succeeded())
	{
		DestructionSound = DestructionAudio.Object;
	}
	// Get the singleton instance of this class
	movementManager = WorldBoundaries::GetInstance();

#ifndef SOUND_CRASHING_BUG
	// Load our Sound Cue for the propeller sound we created in the editor... 
	ConstructorHelpers::FObjectFinder<USoundCue> Cue(
		TEXT("SoundCue'/Game/Audio/police_Cue.police_Cue'")
	);
	// Store a reference to the Cue asset - we'll need it later.
	AudioCue = Cue.Object;
	// Create an audio component
	LoopingIndicatorMusic = CreateDefaultSubobject<UAudioComponent>( 
		TEXT("SaucerAudioComponent")
		);
	// I don't want the sound playing the moment it's created.
	LoopingIndicatorMusic->bAutoActivate = false;

	LoopingIndicatorMusic->SetupAttachment(RootComponent);
#endif

}

ASaucer::~ASaucer()
{
	//LoopingIndicatorMusic->Deactivate();
}

// Called when the game starts or when spawned
void ASaucer::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("ASaucer begin play called"));
	Super::BeginPlay();

	//PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	FTimerHandle TimerHandle_StartOffset;

	// Wait 2 seconds before firing the first time
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_StartOffset, this, &ASaucer::SetCanFire, 2.0f);

	// Set up a timer to change rotation after 5 seconds
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_ChangeRotation, this, &ASaucer::ChangeRotation, 5.0f);
	
	// Store the current rotation
	LastUsedRotation = GetActorRotation();

	// Generate a movement direction specific to the actual class of this instance
	MovementDirection = GenerateMovementDirection();

#ifndef SOUND_CRASHING_BUG
	if (AudioCue->IsValidLowLevelFast() && 
		LoopingIndicatorMusic->IsValidLowLevel()) 
	{
		LoopingIndicatorMusic->SetSound(AudioCue);
		//LoopingIndicatorMusic->Sound = AudioCue;
	}
	else UE_LOG(LogTemp, Warning, TEXT("This is the problem"));
	// Start playing the music
	if (LoopingIndicatorMusic->IsValidLowLevel())
	{
			//LoopingIndicatorMusic->Activate(true);
			LoopingIndicatorMusic->Play(0.0f);
	}
	else 
		UE_LOG(LogTemp, Warning, TEXT("Ofc it wont work"));
#endif

}

// Called every frame
void ASaucer::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	// Fire if possible
	if (CanFire)
	{
		
		EnterTargetingState();

		Fire();

		ExitTargetingState();
	}

	// Update the location of this actor
	FVector location = MovementDirection * DeltaTime;
	this->AddActorLocalOffset(location, true);
	movementManager->CorrectPosition(this);
}

int ASaucer::AwardScore() const
{
	// AwardScore() is overridden by both Small and Large  Saucer, so this should never be called
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
	// The timers should get deleted with the handles
	UGameplayStatics::PlaySoundAtLocation(this, DestructionSound, GetActorLocation());

#ifndef SOUND_CRASHING_BUG
	LoopingIndicatorMusic->Stop();
	//LoopingIndicatorMusic->Deactivate();

#endif
	this->Destroy();
}

void ASaucer::Fire()
{
	// Spawn rotation		
	FRotator SpawnRotation = GetActorRotation();
	// Spawn location
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
		// Set a timer to reset the CanFire flag
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

	// Change the rotation with the given delta
	this->SetActorRotation(GetActorRotation() + rotation);
	
	// Store this new rotation
	this->LastUsedRotation = GetActorRotation();

	// Drop the previous timer
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ChangeRotation);
	// Set up another random timer
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_ChangeRotation,this,&ASaucer::ChangeRotation,FMath::RandRange(3,7));

}

FVector ASaucer::GenerateMovementDirection()
{
	// Should never be called be cause it is overridden
	return FVector(100,0,0);
}

void ASaucer::EnterTargetingState()
{
	// Should never be called be cause it is overridden
	return;
}

void ASaucer::ExitTargetingState()
{
	// Should never be called be cause it is overridden
	return;
}
