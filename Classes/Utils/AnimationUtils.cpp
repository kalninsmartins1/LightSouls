#include "AnimationUtils.h"
#include "GameConsts.h"
#include "tinyxml2/tinyxml2.h"
#include "Utils.h"

using namespace cocos2d;

void AnimationUtils::startSpriteFrameAnimation(Sprite* pSprite, Vector<SpriteFrame*>& spriteFrames,
	float timeBetweenFrames)
{
	// Stop any previously started animation
	pSprite->stopActionByTag(ACTION_ANIM_TAG);

	// Start character animation
	auto animation = Animation::createWithSpriteFrames(spriteFrames, timeBetweenFrames);
	auto animate = Animate::create(animation);
	auto repeatAction = RepeatForever::create(animate);
	repeatAction->setTag(ACTION_ANIM_TAG);

	// Remove any old anim action
	pSprite->stopActionByTag(ACTION_ANIM_TAG);

	// Start new anim
	pSprite->runAction(repeatAction);
}

void AnimationUtils::loadAnimationFrames(const tinyxml2::XMLElement* pElem,
	Vector<SpriteFrame*>& outSpriteFrames, float& outTimeBetweenFrames)
{	
	auto spriteCache = SpriteFrameCache::getInstance();
	const char* plistPath = pElem->Attribute("plistPath");
	spriteCache->addSpriteFramesWithFile(plistPath);

	// Get template frame name
	const char* spriteFrameName = pElem->Attribute("frameNamePattern");

	// Get total frame count
	int frameCount = pElem->IntAttribute("frameCount");

	// Load all the frame for the animation
	char curSpriteFrameName[MAX_SPRITE_NAME_LENGTH] = { 0 };
	for (int i = 0; i < frameCount; i++)
	{
		sprintf(curSpriteFrameName, spriteFrameName, i);
		SpriteFrame* frame = spriteCache->getSpriteFrameByName(curSpriteFrameName);
		outSpriteFrames.pushBack(frame);
	}

	// Set the time between frames
	outTimeBetweenFrames = pElem->FloatAttribute("timeBetweenFrames");
}
