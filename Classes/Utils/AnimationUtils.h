#pragma once

#include "cocos2d.h"

namespace tinyxml2 
{
	class XMLElement;
}

enum class AnimationKind
{
	NONE,
	RUN,
	IDLE,
	ATTACK,
	DODGE
};

class AnimationUtils
{
public:

	static void startSpriteFrameAnimationWithDelayedCallback(cocos2d::Sprite* pSprite,
		cocos2d::Vector<cocos2d::SpriteFrame*>& spriteFrames,
		float timeBetweenFrames, const std::function<void()>& onFinished);

	static void startSpriteFrameAnimation(cocos2d::Sprite* pSprite, 
		cocos2d::Vector<cocos2d::SpriteFrame*>& spriteFrames,
		float timeBetweenFrames);

	static void loadAnimationFrames(const tinyxml2::XMLElement* pElem,
						cocos2d::Vector<cocos2d::SpriteFrame*>& outSpriteFrames,
						float& outTimeBetweenFrames);
};
