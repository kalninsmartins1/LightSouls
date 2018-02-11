#pragma once

#include "IState.h"
#include "World/Entity/Entity.h"
#include "World/Entity/Components/RangedAttackComponent.h"

class AIAgent;

class StateAttack: public IState
{
public:
	StateAttack(AIAgent& agent);
	bool init();

	virtual void onEnter(AIAnimComponent* pAnimComponent) override;
	virtual StateProgress onStep() override;
	virtual void onExit() override;
	AIState getStateType() override;

private:

	void onAttackFinished();

	StateProgress m_curProgress;
	AIAgent& m_agent;
	const Entity& m_targetEntity;
	AttackComponent* m_pAttackComponent;
	AIAnimComponent* m_pAnimComponent;
	bool m_bIsAttackAnimFinished;
};
