// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidsGame.h"
#include "AsteroidsGameGameMode.h"
#include "WorldElements/AsteroidField.h"
#include "PlayerAvatar/SpaceshipPawn.h"
#include "PlayerAvatar/AsteroidsPlayerState.h"
#include "Interface/SimpleHUD.h"

AAsteroidsGameGameMode::AAsteroidsGameGameMode()
{
	DefaultPawnClass = ASpaceshipPawn::StaticClass();
	
	// Player state class was extended by a blueprint, and this setting
	// is now done via the editor (needed a blueprint to get a reference
	// to the game over menu
	//PlayerStateClass = AAsteroidsPlayerState::StaticClass();

	HUDClass = ASimpleHUD::StaticClass();
}

