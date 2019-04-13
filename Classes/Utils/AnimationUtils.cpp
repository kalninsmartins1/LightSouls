#include "AnimationUtils.h"
#include "GameConsts.h"
#include "tinyxml2/tinyxml2.h"
#include "Utils.h"
#include "XML/XMLConsts.h"
#include "World/Entity/Animation/LSAnimation.h"
#include "World/Entity/CustomActions/LSAnimate.h"

const std::map<String, int> AnimationUtils::s_animTypeToId =
{
	{ GameConsts::ANIM_TYPE_RUN, 0 },
	{ GameConsts::ANIM_TYPE_IDLE, 1 },
	{ GameConsts::ANIM_TYPE_DODGE, 2 },
	{ GameConsts::ANIM_TYPE_HURT, 3 },
	{ GameConsts::ANIM_TYPE_ATTACK, 4 },

	{ GameConsts::ANIM_TYPE_ATTACK_COMBO_ONE_SIDE, 5 },
	{ GameConsts::ANIM_TYPE_ATTACK_COMBO_TWO_SIDE, 6 },
	{ GameConsts::ANIM_TYPE_ATTACK_COMBO_THREE_SIDE, 7 },
	{ GameConsts::ANIM_TYPE_ATTACK_COMBO_FOUR_SIDE, 8 },
	{ GameConsts::ANIM_TYPE_ATTACK_COMBO_FIVE_SIDE, 9 },

	{ GameConsts::ANIM_TYPE_ATTACK_COMBO_ONE_DOWN, 10 },
	{ GameConsts::ANIM_TYPE_ATTACK_COMBO_TWO_DOWN, 11 },
	{ GameConsts::ANIM_TYPE_ATTACK_COMBO_THREE_DOWN, 12 },
	{ GameConsts::ANIM_TYPE_ATTACK_COMBO_FOUR_DOWN, 13 },
	{ GameConsts::ANIM_TYPE_ATTACK_COMBO_FIVE_DOWN, 14 },

	{ GameConsts::ANIM_TYPE_ATTACK_COMBO_ONE_UP, 15 },
	{ GameConsts::ANIM_TYPE_ATTACK_COMBO_TWO_UP, 16 },
	{ GameConsts::ANIM_TYPE_ATTACK_COMBO_THREE_UP, 17 },
	{ GameConsts::ANIM_TYPE_ATTACK_COMBO_FOUR_UP, 18 },
	{ GameConsts::ANIM_TYPE_ATTACK_COMBO_FIVE_UP, 19 },

	{ GameConsts::ANIM_TYPE_SIGNAL,	20 },

	{ GameConsts::ANIM_TYPE_RUN_DIR, 21 },
	{ "RunUp", 22 },
	{ "RunDown", 23 },

	{ GameConsts::ANIM_TYPE_IDLE_DIR, 24 },
	{ "IdleUp", 25 },
	{ "IdleDown", 26 },

	{ GameConsts::ANIM_TYPE_DODGE_DIR, 27 },
	{ "DodgeUp", 28 },
	{ "DodgeDown", 29 },

	{ GameConsts::ANIM_TYPE_HURT_DIR, 30 },
	{ "HurtUp", 31 },
	{ "HurtDown", 32 },

	{ GameConsts::ANIM_TYPE_ACT, 33 },
	{ GameConsts::ANIM_TYPE_DISAPPEAR, 34},
};

int AnimationUtils::GetAnimId(String animName)
{
	int animId = -1;
	if (Utils::ContainsKey(s_animTypeToId, animName))
	{
		animId = s_animTypeToId.at(animName);
	}
	else
	{
		CCLOGERROR("No animation %s found !", animName.c_str());
	}

	return animId;
}

void AnimationUtils::GetAnimName(int animId, String& outAnimName)
{
	for (auto animData : s_animTypeToId)
	{
		if (animData.second == animId)
		{
			outAnimName = animData.first;
		}
	}
}

cocos2d::Action* AnimationUtils::StartSpriteFrameAnimationWithCallback(cocos2d::Sprite* sprite,
	const AnimationData& animationData,
	const std::function<void()>& onFinished)
{
	// Stop any previously started animation	
	sprite->stopActionByTag(GameConsts::ACTION_ANIM);

	// Start character animation
	const auto animation = LSAnimation::Create(animationData.frames, animationData.timeBetweenFrames);
	const auto animate = LSAnimate::Create(animation);
	const auto callbackAction = cocos2d::CallFunc::create(onFinished);

	/* According to cocos2d documentation last parameter to Sequnce::create must be nullptr
	 * http://www.cocos2d-x.org/docs/cocos2d-x/en/actions/sequences.html
	*/
	auto sequence = cocos2d::Sequence::create(animate, callbackAction, nullptr);
	sequence->setTag(GameConsts::ACTION_ANIM);

	// Start new anim
	return sprite->runAction(sequence);
}

void AnimationUtils::StartSpriteFrameAnimation(cocos2d::Sprite* sprite, const AnimationData& animationData, bool shouldReverse)
{
	// Stop any previously started animation
	sprite->stopActionByTag(GameConsts::ACTION_ANIM);

	// Start character animation
	const auto animation = LSAnimation::Create(animationData.frames,
		animationData.timeBetweenFrames, shouldReverse);
	auto animateAction = LSAnimate::Create(animation);	
	
	const auto repeatAction = cocos2d::RepeatForever::create(animateAction);
	repeatAction->setTag(GameConsts::ACTION_ANIM);

	// Start new anim	
	sprite->runAction(repeatAction);
}

void AnimationUtils::LoadAnimationFrames(const tinyxml2::XMLElement* animElement,
	AnimationData& outAnimationData)
{
	// Get template frame name
	const char* spriteFrameName = animElement->Attribute(XMLConsts::ANIM_FRAME_NAME_PATTERN_ATTR);
	outAnimationData.shouldBlur = animElement->BoolAttribute(XMLConsts::ANIM_SHOULD_BLUR);

	// Set the time between frames
	const String frameTimeInfo = animElement->Attribute(XMLConsts::ANIM_TIME_BETWEEN_FRAMES_ATTR);
	Utils::ParseFloatArray(frameTimeInfo, outAnimationData.timeBetweenFrames);

	// Get total frame count
	const int frameCount = outAnimationData.timeBetweenFrames.size();

	// Load all the frame for the animation
	auto spriteCache = cocos2d::SpriteFrameCache::getInstance();
	char curSpriteFrameName[GameConsts::MAX_SPRITE_NAME_LENGTH] = { 0 };
	for (int i = 0; i < frameCount; i++)
	{
		sprintf(curSpriteFrameName, spriteFrameName, i);
		cocos2d::SpriteFrame* frame = spriteCache->getSpriteFrameByName(curSpriteFrameName);
		outAnimationData.frames.pushBack(frame);
	}	
}