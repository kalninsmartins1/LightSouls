#pragma once

#include "LightSoulsTypes.h"
#include "Events/AEventData.h"

NS_LIGHTSOULS_BEGIN

class AnimComponent;
class AIAgent;

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
	IDLE,
	SIGNALING,
	PAUSE
};


class AState : public cocos2d::Ref
{
public:	
	virtual AIState GetStateType() const = 0;
	AIState			GetNextStateOnSuccess() const;
	AIState			GetNextStateOnFailure() const;
	static AIState	GetStateFromString(String stateType);

	void			SetNextStateOnSuccess(const AIState& state);
	void			SetNextStateOnFailure(const AIState& state);

	// Called when state is first entered
	virtual void			OnEnter(AnimComponent* animComponent) = 0;

	// Called to progress the state, returns current state progress
	virtual StateProgress	OnStep() = 0;

	// Called when state is exited
	virtual void			OnExit() = 0;

	virtual void			OnEventReceived(const String& receivedEvent, const AEventData& eventData) = 0;

	template<typename T>
	static T*		Create(AIAgent& agent);

private:
	AIState m_nextStateOnSuccess;
	AIState m_nextStateOnFailure;
};

template<typename T>
T* AState::Create(AIAgent& agent)
{
	T* state = new (std::nothrow) T(agent);

	if (state != nullptr)
	{
		state->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(state);
	}

	return state;
}


NS_LIGHTSOULS_END