#include "ActionSequence.h"

bool ActionSequnce::isDone() const
{
	return m_isDone;
}

ActionSequnce* ActionSequnce::Create()
{
	ActionSequnce *ret = new (std::nothrow) ActionSequnce();
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

void ActionSequnce::AddAction(cocos2d::Action* action)
{
	m_actions.pushBack(action);
}

void ActionSequnce::step(float deltaTime)
{
	if (m_actions.size() > 0)
	{
		// Process first action in vector
		Action* action = m_actions.at(0);
		action->step(deltaTime);
		
		// If action has finished then remove it from the vector
		if (action->isDone())
		{
			m_actions.erase(0);
		}
	}
	else
	{
		m_isDone = true;
	}
}