#pragma once

#include "LightSoulsTypes.h"

namespace tinyxml2
{
	class XMLElement;
};

NS_LIGHTSOULS_BEGIN

struct AnimationData
{
	cocos2d::Vector<cocos2d::SpriteFrame*>	frames;
	float									timeBetweenFrames;
};

class AnimationUtils
{
public:
	static int GetAnimId(String animName);

	static cocos2d::Action* StartSpriteFrameAnimationWithCallback(cocos2d::Sprite* pSprite,
		const AnimationData& animationData, const std::function<void()>& onFinished);

	static void StartSpriteFrameAnimation(cocos2d::Sprite* pSprite, 
		const AnimationData& animationData);

	static void LoadAnimationFrames(const tinyxml2::XMLElement* pElem,
		AnimationData& outAnimationData);

private:
	static const std::map<String, int> s_animTypeToId;
};

NS_LIGHTSOULS_END