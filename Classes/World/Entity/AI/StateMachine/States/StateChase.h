#pragma once

#include "AState.h"
#include "World/Entity/Entity.h"

namespace tinyxml2
{
	class XMLElement;
};

using XMLElement = tinyxml2::XMLElement;

class StateChase: public AState
{
public:
	StateChase(AIAgent& agent);

public:
	virtual EAIState		GetStateType() const override;
	
	virtual void			OnEnter(AnimComponent& animComponent) override;
	virtual EStateProgress	OnStep() override;
	virtual void			OnExit() override;	
	virtual void			LoadXMLData(const XMLElement& xmlElement) override;

	void HandleAnimationPlaying(const AIAgent& agent);

private:
	EStateProgress	m_curProgress;
	const Entity*	m_targetEntity;
	AnimComponent*	m_animComponent;
	float			m_chaseRadius;
	float			m_chaseStopDistance;
	Vector2			m_randomTargetOffset;
};

