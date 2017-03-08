// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidsGame.h"
#include "LargeSaucer.h"

FVector ALargeSaucer::GenerateMovementDirection()
{
	return FVector(300,0,0);
}

void ALargeSaucer::EnterTargetingState()
{
	// Set a random rotation
	FRotator newRotation(0,0,0);
	newRotation.Yaw = FMath::FRandRange(-180, 180);
	this->SetActorRelativeRotation(newRotation);
}

void ALargeSaucer::ExitTargetingState()
{
	// Restore the last used roation
	this->SetActorRotation(LastUsedRotation);
}

int ALargeSaucer::AwardScore() const
{
	return 500;
}