#pragma once

#include "LightSoulsTypes.h"
#include "Events/BaseEventData.h"

class AnimComponent;
class AIAgent;
class AState;
enum class EAIState;

class StateMachine
{
public:		
	StateMachine(AIAgent& agent);

public:
	EAIState GetCurrentState() const;

	void SetStartState(EAIState stateType);

	// Should be called once to turn on the state machine
	void Start(AnimComponent& animComponent);
	void AddAvailableState(const EAIState& availableState, const XMLElement& xmlElement);
	void Reset();

	// Should be called periodically to update current state of state machine 
	void OnStep();
	void DispatchEvent(const String& eventType, const BaseEventData& eventData);

private:
	void SwitchState(AState* newState);
	void SwitchState(EAIState newState);
	void OnStateDone();
	void OnStateFailed();

private:
	AIAgent&									m_agent;
	EAIState									m_startState;
	AState*										m_curState;
	AnimComponent*								m_animComponent;
	cocos2d::Map<EAIState, AState*>				m_availableStates;
};


