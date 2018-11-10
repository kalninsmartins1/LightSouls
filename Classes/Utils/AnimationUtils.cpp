#include "AnimationUtils.h"
#include "GameConsts.h"
#include "tinyxml2/tinyxml2.h"
#include "Utils.h"
#include "XML/XMLConsts.h"


const std::map<String, int> AnimationUtils::s_animTypeToId =
{
	{ GameConsts::ANIM_TYPE_RUN, 0 },
	{ GameConsts::ANIM_TYPE_IDLE, 1 },
	{ GameConsts::ANIM_TYPE_DODGE, 2 },
	{ GameConsts::ANIM_TYPE_HURT, 3 },
	{ GameConsts::ANIM_TYPE_ATTACK, 4 },

	{ GameConsts::ANIM_TYPE_ATTACK_COMBO_ONE_FORWARD, 5 },
	{ GameConsts::ANIM_TYPE_ATTACK_COMBO_TWO_FORWARD, 6 },
	{ GameConsts::ANIM_TYPE_ATTACK_COMBO_THREE_FORWARD, 7 },
	{ GameConsts::ANIM_TYPE_ATTACK_COMBO_FOUR_FORWARD, 8 },
	{ GameConsts::ANIM_TYPE_ATTACK_COMBO_FIVE_FORWARD, 9 },

	{ GameConsts::ANIM_TYPE_ATTACK_COMBO_ONE_DOWNWARD, 10 },
	{ GameConsts::ANIM_TYPE_ATTACK_COMBO_TWO_DOWNWARD, 11 },
	{ GameConsts::ANIM_TYPE_ATTACK_COMBO_THREE_DOWNWARD, 12 },
	{ GameConsts::ANIM_TYPE_ATTACK_COMBO_FOUR_DOWNWARD, 13 },
	{ GameConsts::ANIM_TYPE_ATTACK_COMBO_FIVE_DOWNWARD, 14 },

	{ GameConsts::ANIM_TYPE_ATTACK_COMBO_ONE_UPWARD, 15 },
	{ GameConsts::ANIM_TYPE_ATTACK_COMBO_TWO_UPWARD, 16 },
	{ GameConsts::ANIM_TYPE_ATTACK_COMBO_THREE_UPWARD, 17 },
	{ GameConsts::ANIM_TYPE_ATTACK_COMBO_FOUR_UPWARD, 18 },
	{ GameConsts::ANIM_TYPE_ATTACK_COMBO_FIVE_UPWARD, 19 },
	{ GameConsts::ANIM_TYPE_SIGNAL,	20 },
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
	const auto animation = cocos2d::Animation::createWithSpriteFrames(animationData.frames,
		animationData.timeBetweenFrames);
	const auto animate = cocos2d::Animate::create(animation);
	const auto callbackAction = cocos2d::CallFunc::create(onFinished);

	/* According to cocos2d documentation last parameter to Sequnce::create must be nullptr
	 * http://www.cocos2d-x.org/docs/cocos2d-x/en/actions/sequences.html
	*/
	auto sequence = cocos2d::Sequence::create(animate, callbackAction, nullptr);
	sequence->setTag(GameConsts::ACTION_ANIM);

	// Start new anim
	return sprite->runAction(sequence);
}

void AnimationUtils::StartSpriteFrameAnimation(cocos2d::Sprite* sprite, const AnimationData& animation)
{
	using namespace cocos2d;

	// Stop any previously started animation
	sprite->stopActionByTag(GameConsts::ACTION_ANIM);

	// Start character animation
	const auto ccAnimation = cocos2d::Animation::createWithSpriteFrames(animation.frames,
		animation.timeBetweenFrames);
	const auto animateAction = Animate::create(ccAnimation);
	const auto repeatAction = RepeatForever::create(animateAction);
	repeatAction->setTag(GameConsts::ACTION_ANIM);

	// Start new anim
	sprite->runAction(repeatAction);
}

void AnimationUtils::LoadAnimationFrames(const tinyxml2::XMLElement* animElement,
	AnimationData& outAnimationData)
{
	// Get template frame name
	const char* spriteFrameName = animElement->Attribute(XMLConsts::ANIM_FRAME_NAME_PATTERN_ATTR);

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




