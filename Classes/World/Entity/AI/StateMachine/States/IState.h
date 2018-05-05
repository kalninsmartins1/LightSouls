#pragma once

#include "LightSoulsTypes.h"

NS_LIGHTSOULS_BEGIN

class AnimComponent;

enum class StateProgress
{
	NONE,
	IN_PROGRESS,
	DONE,
	FAILED,
};

enum class AIState
{
	NONE,
	CHASE,
	ATTACK,
	PATROL,
};


class IState
{
public:	
	virtual AIState GetStateType() = 0;

	// Called when state is first entered
	virtual void			OnEnter(AnimComponent* animComponent) = 0;

	// Called to progress the state, returns current state progress
	virtual StateProgress	OnStep() = 0;

	// Called when state is exited
	virtual void			OnExit() = 0;

	virtual void			OnEventReceived(const String& receivedEvent) = 0;
};

NS_LIGHTSOULS_END