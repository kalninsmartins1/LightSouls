#pragma once

#include "LightSoulsTypes.h"

namespace tinyxml2
{
	class XMLElement;
};

struct AnimationData
{
	cocos2d::Vector<cocos2d::SpriteFrame*>	frames;	
	std::vector<float>						timeBetweenFrames;
	static constexpr float					secondFrameDelay = 0.06f;	
	static constexpr float					snapShotDelay = 0.10f;
};

class AnimationUtils
{
public:
	static int		GetAnimId(String animName);
	static void		GetAnimName(int animId, String& outAnimName);

	static cocos2d::Action* StartSpriteFrameAnimationWithCallback(cocos2d::Sprite* pSprite,
		const AnimationData& animationData, const std::function<void()>& onFinished);

	static void StartSpriteFrameAnimation(cocos2d::Sprite* pSprite, 
		const AnimationData& animationData, bool shouldReverse = false);

	static void LoadAnimationFrames(const tinyxml2::XMLElement* pElem,
		AnimationData& outAnimationData);

private:
	static const std::map<String, int> s_animTypeToId;
};

