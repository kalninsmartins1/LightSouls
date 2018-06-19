#pragma once

#include "LightSoulsTypes.h"
#include "Events/AEventData.h"

NS_LIGHTSOULS_BEGIN

class AnimComponent;
class AIAgent;
class AState;
enum class AIState;

class StateMachine
{
public:		
	StateMachine(AIAgent& agent);

public:
	void SetStartState(AIState stateType);

	// Should be called once to turn on the state machine
	void Start(AnimComponent* animComponent);
	void AddAvailableState(AIState availableState, AIState stateOnSuccess, AIState stateOnFailure, float timeRestriction);

	// Should be called periodically to update current state of state machine 
	void OnStep();

	void DispatchEvent(const String& eventType, const AEventData& eventData);

private:	
	void SwitchState(AState* newState);
	void SwitchState(AIState newState);
	void OnStateDone();
	void OnStateFailed();

private:
	AIAgent&									m_agent;
	AIState										m_startState;
	AState*										m_curState;
	AnimComponent*								m_animComponent;	
	cocos2d::Map<AIState, AState*>				m_availableStates;
};

NS_LIGHTSOULS_END
