// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidsGame.h"
#include "../PlayerAvatar/AsteroidsPlayerState.h"
#include "SmallSaucer.h"
#define SCORE_THRESHOLD_1 10000
#define SCORE_THRESHOLD_2 30000
void ASmallSaucer::BeginPlay()
{
	Super::BeginPlay();

	// We are playing with a SMALL saucer
	this->SetActorScale3D(FVector(0.5,0.5,0.5));

	// Set the accuracy parameters based on the score
	AAsteroidsPlayerState* CurrentState = Cast<AAsteroidsPlayerState>(PlayerPawn->PlayerState);
	int score = CurrentState->GetScore();

	// Set default values for accuracy
	accuracyRange.second = 20;
	accuracyRange.first = -20;

	if (score > SCORE_THRESHOLD_1)
	{
		accuracyRange.first = -10;
		accuracyRange.second = 10;
	}
	if (score > SCORE_THRESHOLD_2)
	{
		accuracyRange.first = -5;
		accuracyRange.second = 5;
	}

}

FVector ASmallSaucer::GenerateMovementDirection()
{
	return FVector(450,0,0);
}

void ASmallSaucer::EnterTargetingState()
{
	// Face the player
	FRotator newRotation = (PlayerPawn->GetActorLocation() - this->GetActorLocation()).Rotation();

	// Decrease the accuracy a bit
	float accuracyDelta = FMath::RandRange(accuracyRange.first, accuracyRange.second);
	newRotation.Yaw += accuracyDelta;

	// Set the rotation to prepare for fire
	this->SetActorRelativeRotation(newRotation);
}

void ASmallSaucer::ExitTargetingState()
{
	// Reset the rotation to the last used
	this->SetActorRotation(LastUsedRotation);
}
int ASmallSaucer::AwardScore() const
{
	return 1000;
}
