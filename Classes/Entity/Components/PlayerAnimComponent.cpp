#include "PlayerAnimComponent.h"
#include "GameConsts.h"
#include "Utils/Utils.h"
#include "3rdPartyLibs/tinyxml2.h"

using namespace cocos2d;

PlayerAnimComponent * PlayerAnimComponent::create()
{
	PlayerAnimComponent* ret = new (std::nothrow) PlayerAnimComponent();
	if (ret && ret->init())
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

void PlayerAnimComponent::setOwner(cocos2d::Node *owner)
{
	Component::setOwner(owner);
	m_pParent = (Sprite*)owner;
		
	if(m_pParent == nullptr)
	{	
		cocos2d::log("Error: PlayerAimation component is not attached to Sprite !");
	}
}

void PlayerAnimComponent::loadConfig(tinyxml2::XMLNode* pNode)
{
	tinyxml2::XMLElement* pSpriteSheetElem = pNode->FirstChildElement();	

	// Need to use full path to assets, because all configs are Assets folder relative
	m_walkSpriteBatch = SpriteBatchNode::create(
		Utils::appendFullPathToAssetsPath(pSpriteSheetElem->Attribute("path")));
	auto spriteCache = SpriteFrameCache::getInstance();
	spriteCache->addSpriteFramesWithFile(Utils::appendFullPathToAssetsPath(
		pSpriteSheetElem->Attribute("plistPath")));

	// Get template frame name
	const char* spriteFrameName = pSpriteSheetElem->Attribute("frameNamePattern");

	// Get total frame count
	int frameCount = pSpriteSheetElem->IntAttribute("frameCount");	 

	// Load all the frame for the animation
	char curSpriteFrameName[MAX_SPRITE_NAME_LENGTH] = { 0 };
	for (int i = 0; i < frameCount; i++)
	{
		sprintf(curSpriteFrameName, spriteFrameName, i);
		SpriteFrame* frame = spriteCache->getSpriteFrameByName(curSpriteFrameName);
		m_runFrames.pushBack(frame);

		// Save first frame name of walk animation to use as Idle state
		if (i == 0)
		{
			m_baseSpriteFrameName = curSpriteFrameName;
		}
	}

	// init Sprite
	if (!m_pParent->initWithSpriteFrameName(m_baseSpriteFrameName))
	{
		cocos2d::log("Player: %s", "Sprite failed to initialize");
	}
}

void PlayerAnimComponent::startRunAnimation()
{
	// Start character animation
	auto animation = Animation::createWithSpriteFrames(m_runFrames, 0.1f);
	auto animate = Animate::create(animation);
	auto repeatAction = RepeatForever::create(animate);
	repeatAction->setTag(ACTION_ANIM_TAG);

	// remove any old anim action
	m_pParent->stopActionByTag(ACTION_ANIM_TAG);

	// start new anim
	m_pParent->runAction(repeatAction);
}

void PlayerAnimComponent::startDodgeAnimation()
{
	
}

void PlayerAnimComponent::startIdleAnimation()
{
	
}

void PlayerAnimComponent::startAttackAnimation()
{
	
}

void PlayerAnimComponent::stopAttackAnimation()
{
	
}

void PlayerAnimComponent::stopIdleAnimation()
{
	
}

void PlayerAnimComponent::stopDodgeAnimation()
{
	
}

void PlayerAnimComponent::stopRunAnimation()
{
	m_pParent->stopActionByTag(ACTION_ANIM_TAG);
	m_pParent->setSpriteFrame(SpriteFrameCache::getInstance()->
		getSpriteFrameByName(m_baseSpriteFrameName));
}