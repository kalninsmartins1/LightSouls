#pragma once
#include "cocos2d.h"

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
	static PlayerAnimComponent* create();
	virtual void setOwner(cocos2d::Node *owner) override;
	void loadConfig(tinyxml2::XMLNode* pNode);

private:
	void startRunAnimation();
	void startDodgeAnimation();
	void startIdleAnimation();
	void startAttackAnimation();

	void stopAttackAnimation();
	void stopIdleAnimation();
	void stopDodgeAnimation();
	void stopRunAnimation();

private:
	cocos2d::Sprite* m_pParent;
	std::string m_baseSpriteFrameName;
	cocos2d::SpriteBatchNode* m_walkSpriteBatch;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_runFrames;
};
