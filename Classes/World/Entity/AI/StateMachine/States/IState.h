#pragma once

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
	virtual void onEnter() = 0;

	// Called to progress the state, returns current state progress
	virtual StateProgress onStep() = 0;

	// Called when state is exited
	virtual void onExit() = 0;

	virtual AIState getStateType() = 0;
};