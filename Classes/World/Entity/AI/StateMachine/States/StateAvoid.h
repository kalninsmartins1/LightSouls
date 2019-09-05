#pragma once

#include "AState.h"
#include "Classes/Core/Math/Vector2.h"

namespace tinyxml2
{
	class XMLElement;
};

class Entity;
class GenericAttackComponent;

using XMLElement = tinyxml2::XMLElement;

class StateAvoid : public AState
{
public:
	StateAvoid(AIAgent& agent);

public:
	EAIState				GetStateType() const override;

	virtual void			OnEnter(AnimComponent& animComponent) override;
	virtual EStateProgress	OnStep() override;
	virtual void			OnExit() override;
	virtual void			OnEventReceived(const String& receivedEvent, const BaseEventData& eventData) override;
	virtual void			LoadXMLData(const XMLElement& xmlElement) override;

private:
	void					OnRandomTimeExpired();
	void					ProcessAnimations();

private:
	EStateProgress			m_curProgress;	
	GenericAttackComponent* m_attackComponent;
	AnimComponent*			m_animComponent;
	float					m_startAvoidDistance;
	float					m_stopAvoidDistance;
	float					m_failDistance;	
	float					m_randomTime;
	bool					m_isRandomTimeExpired;
	Vector2					m_randomTargetOffset; // Makes agents more disperse
};

