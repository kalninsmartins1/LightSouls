#include "ChaseAction.h"
#include "World/Entity/AI/AIAgent.h"

NS_LIGHTSOULS_BEGIN

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
	
	float distanceToTarget = toTarget.length();

	if (distanceToTarget <= m_agent.GetChaseStopDistance() ||	// Target has been caught
		distanceToTarget >= m_agent.GetChaseRadius()) // Target run off
	{
	
		m_isDone = true;
	}	
}

NS_LIGHTSOULS_END
