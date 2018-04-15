#include "ChaseAction.h"

ChaseAction* ChaseAction::Create(const Entity& chaseTarget, AIAgent& agent)
{
	ChaseAction *ret = new (std::nothrow) ChaseAction(chaseTarget, agent);
	if (ret != nullptr)
	{
		ret->autorelease();		
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	
	return ret;
}

ChaseAction::ChaseAction(const Entity& chaseTarget, AIAgent& agent):
	m_chaseTarget(chaseTarget),	
	m_agent(agent),
	m_isDone(false)
{
}

ChaseAction::~ChaseAction()
{

}

bool ChaseAction::isDone() const
{
	return m_isDone;
}

void ChaseAction::step(float dt)
{
	// Move agent towards target location
	const Vector2& currentPosition = m_agent.getPosition();
	Vector2 toTarget = m_chaseTarget.getPosition() - currentPosition;
	const Vector2& toTargetNormalized = toTarget.getNormalized();

	// Update agent move direction
	m_agent.SetMoveDirection(toTargetNormalized);
	
	// Move the agent
	m_agent.Move();

	// Check if attack distance has been reached
	if (toTarget.length() <= m_agent.GetChaseStopDistance())
	{
		// Attack distance has been reached - state finished
		m_isDone = true;
	}
}
