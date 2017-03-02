// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidsGame.h"
#include "AsteroidsGameGameMode.h"
#include "WorldElements/AsteroidField.h"
#include "PlayerAvatar/SpaceshipPawn.h"

AAsteroidsGameGameMode::AAsteroidsGameGameMode()
{
	DefaultPawnClass = ASpaceshipPawn::StaticClass();
	
}

