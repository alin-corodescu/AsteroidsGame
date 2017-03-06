// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WorldElements/Saucer.h"
#include <utility>
#include "SmallSaucer.generated.h"

/**
 * 
 */
UCLASS()
class ASTEROIDSGAME_API ASmallSaucer : public ASaucer
{
	GENERATED_BODY()
public:
	virtual int AwardScore() const override;

	virtual void BeginPlay() override;
protected:
	// Generates a Movement Direction vector specific to the type of the object
	virtual FVector GenerateMovementDirection() override;
	/**
	*	Modifies the actor rotation using it's targeting logic
	*	To be used in conjuction with ExitTargetingState()
	*	/sa ExitTargetingState
	*/
	virtual void EnterTargetingState() override;
	/**
	*	Reverts the actor rotation to the one used before calling
	*	EnterTargetingState(), has no effect otherwise.
	*/
	virtual void ExitTargetingState() override;	
private:
	// Range of the accuracy angle, dependent on the current score
	std::pair<float,float> accuracyRange;
};
