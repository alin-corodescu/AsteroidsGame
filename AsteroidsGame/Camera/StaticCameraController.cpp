// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidsGame.h"
#include "StaticCameraController.h"
#include "../WorldElements/WorldBoundaries.h"

// Sets default values
AStaticCameraController::AStaticCameraController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the camera component to be used as RootComponent of the actor
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));

	// Set the actual RootComponent
	RootComponent = CameraComponent;

	FVector MyFVector(0.0f, 0.0f, 2000.0f);
	CameraComponent->SetWorldLocation(MyFVector);
	FRotator MyFRotator(-90.0f, 0.0f, -90.0f);
	CameraComponent->SetWorldRotation(MyFRotator);
	CameraComponent->SetProjectionMode(ECameraProjectionMode::Type::Orthographic);
	CameraComponent->OrthoWidth = 3000;
	CameraComponent->bConstrainAspectRatio = true;

	WorldBoundaries::GetInstance()->SetUpLimits(CameraComponent);
}

// Called when the game starts or when spawned
void AStaticCameraController::BeginPlay()
{
	Super::BeginPlay();

	UWorld* const World = GetWorld();

	if (World != NULL)
	{
		APlayerController* const PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			PlayerController->SetViewTarget(this);
		}
	}



}

// Called every frame
void AStaticCameraController::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

