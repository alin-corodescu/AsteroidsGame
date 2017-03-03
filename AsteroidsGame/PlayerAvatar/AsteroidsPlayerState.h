// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "AsteroidsPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ASTEROIDSGAME_API AAsteroidsPlayerState : public APlayerState
{
	GENERATED_BODY()
	
private:
	int CurrentScore;

	int NumberOfLives;

public:
	int GetScore();
	void SetScore(int newScore);
	void modifyScore(int amount);

	int GetNumberOfLives();
	void SetNumberOfLives(int numberOfLives);
	void addLives(int count);
	
	
	
};
