// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "AsteroidsPlayerState.generated.h"

/**
 * Class holding information regarding the current player state.
 *	Stores information about the current score and the number of lives the player currently has
 */
UCLASS()
class ASTEROIDSGAME_API AAsteroidsPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int CurrentScore; //!< Value of the current score

	int NumberOfLives; //!< Number of lives left

public:

	int GetScore(); //!< Getter for score
	void SetScore(int newScore); //!< Setter for score
	/**
	*	Updates the current score by the specified amount. 
	*	Can be negative.
	*	@param [in] amount the amount to be added to the score
	*/
	void modifyScore(int amount); //!< Function used to update the score by a specific amount

	int GetNumberOfLives(); //!< Getter for the number of lives
	void SetNumberOfLives(int numberOfLives); //!< Setter for the number of lives
	/**
	*	Updates the number of lives by adding the amount specified
	*	by the argument. The argument can be negative. If the lives reach 0,
	*	finishes the game
	*	@param [in] count the number of lives to be added
	*/
	void modifyLives(int count);


	//! Reference UMG Asset in the Editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> GameOverAsset;

	//! Variable to hold the widget After Creating it.
	UUserWidget* GameOverWidget;
};
