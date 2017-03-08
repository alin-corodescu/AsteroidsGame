// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "SimpleHUD.generated.h"

/**
 * Class used to draw the HUD of the game. Displays current score and lives
 */
UCLASS()
class ASTEROIDSGAME_API ASimpleHUD : public AHUD
{
	GENERATED_BODY()

	class AAsteroidsPlayerState* CurrentState;

	void DrawScore(); //!< Method used to display the current score
	void DrawLives(); //!< Method used to represent the number of lives

	UPROPERTY()
	UTexture2D* LivesImageAsset; //!< The asset containing the image to be used for lives

	UPROPERTY()
	FCanvasIcon LivesIcon; //!< Icon used to display the number of lives

public:

	ASimpleHUD(); //!< Default Constructor

	UPROPERTY()
	UFont* HUDFont; //!< Font used to display the score

	virtual void DrawHUD() override; //!< Inherited from AHUD classs
	
	
};
