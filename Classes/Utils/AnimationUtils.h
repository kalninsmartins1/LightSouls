#pragma once

#include "Classes/External/CocosEngine.h"

namespace tinyxml2
{
	class XMLElement;
};

class String;

using XMLElement = tinyxml2::XMLElement;

struct AnimationData
{
	cc::Vector<cc::SpriteFrame*>			frames;	
	std::vector<float>						timeBetweenFrames;
	bool									shouldBlur;
	static constexpr float					secondFrameDelay = 0.06f;	
	static constexpr float					snapShotDelay = 0.10f;
};

class AnimationUtils
{
public:
	static int		GetAnimId(const String& animName);
	static void		GetAnimName(int animId, String& outAnimName);

	static cc::Action* StartSpriteFrameAnimationWithCallback(cc::Sprite* pSprite,
		const AnimationData& animationData, const std::function<void()>& onFinished);

	static void StartSpriteFrameAnimation(cc::Sprite* pSprite, 
		const AnimationData& animationData, bool shouldReverse = false);

	static void LoadAnimationFrames(const XMLElement* pElem,
		AnimationData& outAnimationData);

private:
	static const std::map<String, int> s_animTypeToId;
};

