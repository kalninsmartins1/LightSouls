#pragma once

#include "cocos2d.h"

namespace tinyxml2 {
	class XMLNode;
}

class AIAnimComponent : public cocos2d::Component
{
public:
	static AIAnimComponent* Create(cocos2d::Sprite& sprite);

	// Load animation configuration from xml file
	void LoadConfig(tinyxml2::XMLNode* pNode);

	void PlayIdleAnimation();
	void PlayRunAnimation();
	void playAttackAnimationWithCallback(std::function<void()> onFinished);

private:
	AIAnimComponent(cocos2d::Sprite& sprite);

private:
	cocos2d::Sprite&	m_agentSprite;
	float				m_timeBetweenIdleFrames;	
	float				m_timeBetweenRunFrames;	
	float				m_timeBetweenAttackFrames;		

	// Animation containers	
	cocos2d::Vector<cocos2d::SpriteFrame*> m_idleFrames;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_runFrames;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_attackFrames;
};
