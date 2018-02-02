#include "ChaseAction.h"


ChaseAction* ChaseAction::create(const Entity& chaseTarget, AIAgent& agent)
{
	ChaseAction *ret = new (std::nothrow) ChaseAction(chaseTarget, agent);
	if (ret && ret->init())
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
	m_bIsDone(false)
{
}

bool ChaseAction::init()
{
	return true;
}

ChaseAction::~ChaseAction()
{
	Action::~Action();
}

bool ChaseAction::isDone() const
{
	return m_bIsDone;
}

void ChaseAction::step(float dt)
{
	// Move agent towards taget location
	const cocos2d::Vec2& currentPosition = m_agent.getPosition();
	cocos2d::Vec2 toTarget = m_chaseTarget.getPosition() - currentPosition;

	m_agent.setPosition(currentPosition + toTarget.getNormalized() *
		m_agent.getCurrentMoveSpeed() * dt);

	// Check if attack distance has been reached
	if (toTarget.length() <= m_agent.getAttackRadius())
	{
		// Attack distance has been reached - state finished
		m_bIsDone = true;
	}
}
