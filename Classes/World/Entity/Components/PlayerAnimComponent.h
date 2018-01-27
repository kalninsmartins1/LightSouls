#pragma once

#include "2d/CCComponent.h"
#include "2d/CCSprite.h"
#include "Utils/AnimationUtils.h"

class Player;

namespace tinyxml2 {
	class XMLNode;
}

typedef std::function<void()> AnimationCallback;

enum class LightAttackStage: unsigned short int
{
	NONE = 0,
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
};

/*
 * Controls animation for player object. Depends on parent being of
 * type sprite. Since otherwise there is nothing to animate.
 */
class PlayerAnimComponent: public cocos2d::Component
{
public:
	static PlayerAnimComponent* create(cocos2d::Sprite& sprite);

	AnimationKind getCurrentlyLoopingAnimation() const;

	// Load animation configuration from xml file
	void loadConfig(tinyxml2::XMLNode* pNode);

	// Public interface to start animations
	void playLightAttackAnimation(LightAttackStage stage, 
		const AnimationCallback& callback);	
	void playHurtAnimation();
	
	void loopRunAnimation();
	void loopIdleAnimation();
	void loopDodgeAnimation();

private:
	PlayerAnimComponent(cocos2d::Sprite& sprite);
	void onPlayingNoLoopingAnim();

	AnimationKind m_currentlyLoopingAnim;
	cocos2d::Sprite& m_playerSprite;
	
	// Animation containers	
	cocos2d::Vector<cocos2d::SpriteFrame*> m_runFrames;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_idleFrames;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_hurtFrames;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_dodgeFrames;

	cocos2d::Vector<cocos2d::SpriteFrame*> m_lightAttackOneFrames;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_lightAttackTwoFrames;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_lightAttackThreeFrames;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_lightAttackFourFrames;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_lightAttackFiveFrames;

	// Represents time how long single frame is shown
	float m_timeBetweenRunFrames;
	float m_timeBetweenIdleFrames;
	float m_timeBetweenHurtFrames;
	float m_timeBetweenDodgeFrames;

	float m_timeBetweenLightAttackOneFrames;
	float m_timeBetweenLightAttackTwoFrames;
	float m_timeBetweenLightAttackThreeFrames;
	float m_timeBetweenLightAttackFourFrames;
	float m_timeBetweenLightAttackFiveFrames;
};
