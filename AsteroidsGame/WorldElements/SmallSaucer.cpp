// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidsGame.h"
#include "../PlayerAvatar/AsteroidsPlayerState.h"
#include "SmallSaucer.h"
#define SCORE_THRESHOLD_1 10000
#define SCORE_THRESHOLD_2 30000
void ASmallSaucer::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Small saucer begin called"));
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
	return FVector(200,0,0);
}

void ASmallSaucer::EnterTargetingState()
{
	FRotator newRotation = (PlayerPawn->GetActorLocation() - this->GetActorLocation()).Rotation();

	this->SetActorRelativeRotation(newRotation);
}

void ASmallSaucer::ExitTargetingState()
{
	this->SetActorRotation(LastUsedRotation);
}
int ASmallSaucer::AwardScore() const
{
	return 1000;
}
