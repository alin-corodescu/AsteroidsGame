// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidsGame.h"
#include "AsteroidsPlayerState.h"

int AAsteroidsPlayerState::GetScore()
{
	return CurrentScore;
}

void AAsteroidsPlayerState::SetScore(int newScore)
{
	CurrentScore = newScore;
}

void AAsteroidsPlayerState::modifyScore(int amount)
{
	CurrentScore += amount;
}

int AAsteroidsPlayerState::GetNumberOfLives()
{
	return NumberOfLives;
}

void AAsteroidsPlayerState::SetNumberOfLives(int numberOfLives)
{
	NumberOfLives = numberOfLives;
}

void AAsteroidsPlayerState::addLives(int count)
{
	NumberOfLives += count;
}
