#pragma once

#include "../AState.h"



class GenericAttackComponent;
class Entity;

class StateLineAttack : public AState
{
public:
	StateLineAttack(AIAgent& aiAgent);

public:
	virtual EAIState		GetStateType() const override;	

	virtual void			OnEnter(AnimComponent * animComponent) override;
	virtual EStateProgress	OnStep() override;
	virtual void			OnExit() override;
	virtual void			OnEventReceived(const String & receivedEvent, const AEventData & eventData) override;	
	virtual void			LoadXMLData(const XMLElement* xmlElement) override;

private:
	void OnSuccessfulAttack();
	void SetTargetPosition(const Vector2& targetPos);

private:
	EStateProgress			m_curProgress;	
	Entity*					m_targetEntity;
	Vector2					m_targetPos;
	GenericAttackComponent*	m_attackComponent;
	float					m_moveSpeed;
	float					m_deliverDamageDistance;
	float					m_arriveDistance;
};

