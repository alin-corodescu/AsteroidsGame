// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidsGame.h"
#include "WorldBoundaries.h"

WorldBoundaries* WorldBoundaries::instance = NULL;

WorldBoundaries::WorldBoundaries()
{
	Top = Bottom = Right = Left = 0;
}

WorldBoundaries::~WorldBoundaries()
{
}

WorldBoundaries * WorldBoundaries::GetInstance()
{
	// Typical singleton set-up
	if (instance == NULL)
	{
		instance = new WorldBoundaries();
	}
	return instance;
}

void WorldBoundaries::SetUpLimits(const UCameraComponent * camera)
{
	// Compute the limits of the world based on the camera view
	float Width = camera->OrthoWidth;
	float Height = Width / camera->AspectRatio;
	Left = -Width / 2.0;
	Right = Width / 2.0;
	Top = Height / 2.0;
	Bottom = -Height / 2.0;

}

bool WorldBoundaries::CorrectPosition(AActor* actor)
{
	FVector actorLoc;
	// Get the actors current location.
	actorLoc = actor->GetActorLocation();
	bool edgeOfWorld = false;

	// Test if the actor is outside the world edges
	if (actorLoc.X < Left)
	{
		actorLoc.X = Right;
		edgeOfWorld = true;
	}
	if (actorLoc.X > Right)
	{
		actorLoc.X = Left;
		edgeOfWorld = true;
	}
	if (actorLoc.Y < Bottom)
	{
		actorLoc.Y = Top;
		edgeOfWorld = true;
	}
	if (actorLoc.Y > Top)
	{
		actorLoc.Y = Bottom;
		edgeOfWorld = true;
	}
	// Move the actor.
	if (edgeOfWorld == true)
	{
		/* If we are jumping to the other side of the world we need to
		switch the sweep off. */
		actor->SetActorLocation(actorLoc, false);
	}
	return edgeOfWorld;
}
