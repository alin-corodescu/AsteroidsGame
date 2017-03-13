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

void AAsteroidsPlayerState::modifyLives(int count)
{
	NumberOfLives += count;
	if (NumberOfLives < 0)
	{
		APlayerController* const player = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));


		if (GameOverAsset) // Check if the Asset is assigned in the blueprint.
		{
			// Create the widget and store it.
			GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverAsset);

			if (GameOverWidget)
			{
				// Add it to the view port
				GameOverWidget->AddToViewport();
			}

		}
		player->SetPause(true);
	}
}
