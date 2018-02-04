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

	virtual void onEnter() override;
	virtual StateProgress onStep() override;
	virtual void onExit() override;
	AIState getStateType() override;

private:
	StateProgress m_curProgress;
	AIAgent& m_agent;
	const Entity& m_targetEntity;
	AttackComponent* m_pAttackComponent;
};
