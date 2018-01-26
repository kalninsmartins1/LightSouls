#pragma once

#include "cocos2d.h"

namespace tinyxml2 {
	class XMLNode;
}

class AgentAnimationComponent : public cocos2d::Component
{
public:
	static AgentAnimationComponent* create(cocos2d::Sprite& sprite);

	// Load animation configuration from xml file
	void loadConfig(tinyxml2::XMLNode* pNode);

	void playIdleAnimation();

private:
	AgentAnimationComponent(cocos2d::Sprite& sprite);

	cocos2d::Sprite& m_agentSprite;
	float m_timeBetweenIdleFrames;	

	// Animation containers	
	cocos2d::Vector<cocos2d::SpriteFrame*> m_idleFrames;

};
