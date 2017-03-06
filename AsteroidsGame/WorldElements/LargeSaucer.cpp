// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidsGame.h"
#include "LargeSaucer.h"

FVector ALargeSaucer::GenerateMovementDirection()
{
	return FVector(100,0,0);
}

void ALargeSaucer::EnterTargetingState()
{
	FRotator newRotation;
	newRotation.Yaw = FMath::FRandRange(-180, 180);
	this->SetActorRelativeRotation(newRotation);
}

void ALargeSaucer::ExitTargetingState()
{
	this->SetActorRotation(LastUsedRotation);
}

int ALargeSaucer::AwardScore() const
{
	return 500;
}