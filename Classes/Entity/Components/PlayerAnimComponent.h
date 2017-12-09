#pragma once
#include "cocos2d.h"
#include "tinyxml2/tinyxml2.h"

namespace tinyxml2 {
	class XMLNode;
}

/*
 * Controls animation for player object. Depends on parent being of
 * type sprite. Since otherwise there is nothing to animate.
 */
class PlayerAnimComponent: public cocos2d::Component
{
public:
	CREATE_FUNC(PlayerAnimComponent);
	
	virtual void setOwner(cocos2d::Node *owner) override;

	// Load animation configuration from xml file
	void loadConfig(tinyxml2::XMLNode* pNode);

	// Public interface to trigger animations
	void startRunAnimation();
	void startDodgeAnimation();
	void startIdleAnimation();
	void startAttackAnimation();

private:
	cocos2d::Sprite* m_pParent;
	
	// Animation containers	
	cocos2d::Vector<cocos2d::SpriteFrame*> m_runFrames;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_idleFrames;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_hurtFrames;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_dodgeFrames;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_attackFrames;

	// Represents time how long single frame is shown
	float m_timeBetweenRunFrames;
	float m_timeBetweenIdleFrames;
	float m_timeBetweenHurtFrames;
	float m_timeBetweenDodgeFrames;
	float m_timeBetweenAttackFrames;
};
