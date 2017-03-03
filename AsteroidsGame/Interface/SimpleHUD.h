// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "SimpleHUD.generated.h"

/**
 * 
 */
UCLASS()
class ASTEROIDSGAME_API ASimpleHUD : public AHUD
{
	GENERATED_BODY()

		class AAsteroidsPlayerState* CurrentState;

	void DrawScore();
	void DrawLives();

	UPROPERTY()
	UTexture2D* LivesImageAsset; //to be renamed;

	UPROPERTY()
	FCanvasIcon LivesIcon;

public:

	ASimpleHUD();

	UPROPERTY()
	UFont* HUDFont;

	virtual void DrawHUD() override;
	
	
};
