// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WorldElements/Saucer.h"
#include "LargeSaucer.generated.h"

/**
 * Class for large Saucers
 */
UCLASS()
class ASTEROIDSGAME_API ALargeSaucer : public ASaucer
{
	GENERATED_BODY()
public:
	//! Inherited from the IAwardsScoreInterface
	virtual int AwardScore() const override;

protected:
	//! Generates a Movement Direction vector specific to the type of the object
	virtual FVector GenerateMovementDirection() override;

	/**
	*	Modifies the actor rotation using it's targeting logic
	*	To be used in conjuction with ExitTargetingState(). 
	*   Large saucers shoots in random direction
	*	@see ExitTargetingState
	*/
	virtual void EnterTargetingState() override;
	/**
	*	Reverts the actor rotation to the one used before calling
	*	EnterTargetingState(), has no effect otherwise.
	*	@see EnterTargetingState
	*/
	virtual void ExitTargetingState() override;
	
	
	
};
