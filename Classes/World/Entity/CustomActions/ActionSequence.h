#pragma once

#include <vector>
#include "Classes/External/CocosEngine.h"

class ActionSequnce : public cocos2d::Action
{
public:
	
	virtual bool isDone() const override;

	static ActionSequnce* Create();

	void			AddAction(cocos2d::Action* action);
	virtual void	step(float deltaTime) override;

private:
	cocos2d::Vector<cocos2d::Action*>	m_actions;
	bool								m_isDone;
};