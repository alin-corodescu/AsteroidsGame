#pragma once

#include "AwardsScoreInterface.generated.h"

UINTERFACE(NotBlueprintable, meta = (CannotImplementInterfaceInBlueprint))
class UAwardsScoreInterface : public UInterface
{
	GENERATED_BODY()
};

class IAwardsScoreInterface
{
	GENERATED_BODY()

public:
	/** React to a trigger volume activating this object. Return true if the reaction succeeds. */
	UFUNCTION(BlueprintCallable, BlueprintCallable, Category = "Score")
	virtual int AwardScore() const = 0;
};