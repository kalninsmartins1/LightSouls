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
	{ ANIM_TYPE_ATTACK_COMBO_ONE, 5 },
	{ ANIM_TYPE_ATTACK_COMBO_TWO, 6 },
	{ ANIM_TYPE_ATTACK_COMBO_THREE, 7 },
	{ ANIM_TYPE_ATTACK_COMBO_FOUR, 8 },
	{ ANIM_TYPE_ATTACK_COMBO_FIVE, 9 },
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

void AnimationUtils::StartSpriteFrameAnimationWithCallback(cocos2d::Sprite* pSprite,
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
	pSprite->runAction(sequence);
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

void AnimationUtils::LoadAnimationFrames(const tinyxml2::XMLElement* pElem,
	AnimationData& outAnimationData)
{
	auto spriteCache = cocos2d::SpriteFrameCache::getInstance();
	const char* plistPath = pElem->Attribute(XML_ANIM_PLIST_PATH_ATTR);
	spriteCache->addSpriteFramesWithFile(plistPath);

	// Get template frame name
	const char* spriteFrameName = pElem->Attribute(XML_ANIM_FRAME_NAME_PATTERN_ATTR);

	// Get total frame count
	const int frameCount = pElem->IntAttribute(XML_ANIM_FRAME_COUNT_ATTR);

	// Load all the frame for the animation
	char curSpriteFrameName[MAX_SPRITE_NAME_LENGTH] = { 0 };
	for (int i = 0; i < frameCount; i++)
	{
		sprintf(curSpriteFrameName, spriteFrameName, i);
		cocos2d::SpriteFrame* frame = spriteCache->getSpriteFrameByName(curSpriteFrameName);
		outAnimationData.frames.pushBack(frame);
	}

	// Set the time between frames
	outAnimationData.timeBetweenFrames = pElem->FloatAttribute(XML_ANIM_TIME_BETWEEN_FRAMES_ATTR);
}

NS_LIGHTSOULS_END


