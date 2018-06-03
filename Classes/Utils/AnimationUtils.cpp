#include "AnimationUtils.h"
#include "GameConsts.h"
#include "tinyxml2/tinyxml2.h"
#include "Utils.h"
#include "XML/XMLConsts.h"


NS_LIGHTSOULS_BEGIN

const std::map<String, int> LightSouls::AnimationUtils::s_animTypeToId =
{
	{ ANIM_TYPE_RUN, 0 },
	{ ANIM_TYPE_IDLE, 1 },
	{ ANIM_TYPE_DODGE, 2 },
	{ ANIM_TYPE_HURT, 3 },
	{ ANIM_TYPE_ATTACK, 4 },

	{ ANIM_TYPE_ATTACK_COMBO_ONE_FORWARD, 5 },
	{ ANIM_TYPE_ATTACK_COMBO_TWO_FORWARD, 6 },
	{ ANIM_TYPE_ATTACK_COMBO_THREE_FORWARD, 7 },
	{ ANIM_TYPE_ATTACK_COMBO_FOUR_FORWARD, 8 },
	{ ANIM_TYPE_ATTACK_COMBO_FIVE_FORWARD, 9 },

	{ ANIM_TYPE_ATTACK_COMBO_ONE_DOWNWARD, 10 },
	{ ANIM_TYPE_ATTACK_COMBO_TWO_DOWNWARD, 11 },
	{ ANIM_TYPE_ATTACK_COMBO_THREE_DOWNWARD, 12 },
	{ ANIM_TYPE_ATTACK_COMBO_FOUR_DOWNWARD, 13 },
	{ ANIM_TYPE_ATTACK_COMBO_FIVE_DOWNWARD, 14 },

	{ ANIM_TYPE_ATTACK_COMBO_ONE_UPWARD, 15 },
	{ ANIM_TYPE_ATTACK_COMBO_TWO_UPWARD, 16 },
	{ ANIM_TYPE_ATTACK_COMBO_THREE_UPWARD, 17 },
	{ ANIM_TYPE_ATTACK_COMBO_FOUR_UPWARD, 18 },
	{ ANIM_TYPE_ATTACK_COMBO_FIVE_UPWARD, 19 },
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

cocos2d::Action* AnimationUtils::StartSpriteFrameAnimationWithCallback(cocos2d::Sprite* pSprite,
	const AnimationData& animationData,
	const std::function<void()>& onFinished)
{
	// Stop any previously started animation
	pSprite->stopActionByTag(ACTION_ANIM_TAG);

	// Start character animation
	const auto animation = cocos2d::Animation::createWithSpriteFrames(animationData.frames,
		animationData.timeBetweenFrames);
	const auto animate = cocos2d::Animate::create(animation);
	const auto callbackAction = cocos2d::CallFunc::create(onFinished);

	/* According to cocos2d documentation last parameter to Sequnce::create must be nullptr
	 * http://www.cocos2d-x.org/docs/cocos2d-x/en/actions/sequences.html
	*/
	auto sequence = cocos2d::Sequence::create(animate, callbackAction, nullptr);
	sequence->setTag(ACTION_ANIM_TAG);

	// Start new anim
	return pSprite->runAction(sequence);
}

void AnimationUtils::StartSpriteFrameAnimation(cocos2d::Sprite* sprite, const AnimationData& animation)
{
	using namespace cocos2d;

	// Stop any previously started animation
	sprite->stopActionByTag(ACTION_ANIM_TAG);

	// Start character animation
	const auto ccAnimation = cocos2d::Animation::createWithSpriteFrames(animation.frames,
		animation.timeBetweenFrames);
	const auto animateAction = Animate::create(ccAnimation);
	const auto repeatAction = RepeatForever::create(animateAction);
	repeatAction->setTag(ACTION_ANIM_TAG);

	// Start new anim
	sprite->runAction(repeatAction);
}

void AnimationUtils::LoadAnimationFrames(const tinyxml2::XMLElement* animElement,
	AnimationData& outAnimationData)
{
	// Get template frame name
	const char* spriteFrameName = animElement->Attribute(XML_ANIM_FRAME_NAME_PATTERN_ATTR);

	// Get total frame count
	const int frameCount = animElement->IntAttribute(XML_ANIM_FRAME_COUNT_ATTR);

	// Load all the frame for the animation
	auto spriteCache = cocos2d::SpriteFrameCache::getInstance();
	char curSpriteFrameName[MAX_SPRITE_NAME_LENGTH] = { 0 };
	for (int i = 0; i < frameCount; i++)
	{
		sprintf(curSpriteFrameName, spriteFrameName, i);
		cocos2d::SpriteFrame* frame = spriteCache->getSpriteFrameByName(curSpriteFrameName);
		outAnimationData.frames.pushBack(frame);
	}

	// Set the time between frames
	outAnimationData.timeBetweenFrames = animElement->FloatAttribute(XML_ANIM_TIME_BETWEEN_FRAMES_ATTR);
}

NS_LIGHTSOULS_END


