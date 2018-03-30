#pragma once
#include "World/Entity/Components/AIAnimComponent.h"

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
	// Called when state is first entered
	virtual void OnEnter(AIAnimComponent* pAnimComponent) = 0;

	// Called to progress the state, returns current state progress
	virtual StateProgress OnStep() = 0;

	// Called when state is exited
	virtual void OnExit() = 0;

	virtual AIState GetStateType() = 0;
};