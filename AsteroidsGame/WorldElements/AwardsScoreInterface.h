#pragma once

#include "AwardsScoreInterface.generated.h"
/**
* Interface implemented by the actors which award score to the player
*/
UINTERFACE(NotBlueprintable, meta = (CannotImplementInterfaceInBlueprint))
class UAwardsScoreInterface : public UInterface
{
	GENERATED_BODY()
};

class IAwardsScoreInterface
{
	GENERATED_BODY()

public:
	/** @return the score to be awarded by the object */
	UFUNCTION(BlueprintCallable, BlueprintCallable, Category = "Score")
	virtual int AwardScore() const = 0;
};