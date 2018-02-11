#include "AnimationUtils.h"
#include "XML/XMLConsts.h"
#include "GameConsts.h"
#include "tinyxml2/tinyxml2.h"
#include "Utils.h"

using namespace cocos2d;

void AnimationUtils::startSpriteFrameAnimationWithCallback(Sprite* pSprite,
	Vector<SpriteFrame*>& spriteFrames, float timeBetweenFrames, 
	const std::function<void()>& onFinished)
{
	// Stop any previously started animation
	pSprite->stopActionByTag(ACTION_ANIM_TAG);

	// Start character animation
	const auto pAnimation = Animation::createWithSpriteFrames(spriteFrames,
		timeBetweenFrames);
	const auto pAnimate = Animate::create(pAnimation);
	const auto pCallback = CallFunc::create(onFinished);

	/* According to cocos2d documentation last parameter to Sequnce::create must be nullptr
	 * http://www.cocos2d-x.org/docs/cocos2d-x/en/actions/sequences.html
	*/
	Sequence* pSequence = Sequence::create(pAnimate, pCallback, nullptr);
	pSequence->setTag(ACTION_ANIM_TAG);

	// Start new anim
	pSprite->runAction(pSequence);
}

void AnimationUtils::startSpriteFrameAnimation(Sprite* pSprite, Vector<SpriteFrame*>& spriteFrames,
	float timeBetweenFrames)
{
	// Stop any previously started animation
	pSprite->stopActionByTag(ACTION_ANIM_TAG);

	// Start character animation
	const auto pAnimation = Animation::createWithSpriteFrames(spriteFrames, timeBetweenFrames);
	const auto pAnimate = Animate::create(pAnimation);
	const auto pRepeatAction = RepeatForever::create(pAnimate);
	pRepeatAction->setTag(ACTION_ANIM_TAG);

	// Start new anim
	pSprite->runAction(pRepeatAction);
}

void AnimationUtils::loadAnimationFrames(const tinyxml2::XMLElement* pElem,
	Vector<SpriteFrame*>& outSpriteFrames, float& outTimeBetweenFrames)
{	
	auto spriteCache = SpriteFrameCache::getInstance();
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
		SpriteFrame* frame = spriteCache->getSpriteFrameByName(curSpriteFrameName);
		outSpriteFrames.pushBack(frame);
	}

	// Set the time between frames
	outTimeBetweenFrames = pElem->FloatAttribute(XML_ANIM_TIME_BETWEEN_FRAMES_ATTR);
}
