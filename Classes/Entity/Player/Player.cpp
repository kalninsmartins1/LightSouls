#include "Player.h"
#include "3rdPartyLibs\tinyxml2.h"
#include "Utils/Utils.h"
#include "GameConsts.h"

using namespace cocos2d;

Player* Player::create(const char* pathToXML)
{
	Player *pPlayer = new (std::nothrow) Player();
	if (pPlayer && pPlayer->init(pathToXML))
	{
		pPlayer->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pPlayer);
	}

	return pPlayer;
}

bool Player::init(const char* pathToXML)
{
	initGraphics(pathToXML);
	setAnchorPoint(Vec2(0.5, 0.5));

	Size size = Director::getInstance()->getWinSize();

	// Position player in middle of the sceen
	setPosition(size.width / 2, size.height / 2);

	// Register for input events	
	EventListenerKeyboard* pKeyboardListener = EventListenerKeyboard::create();
	pKeyboardListener->onKeyReleased = CC_CALLBACK_2(onKeyboardKeyUp, this);
	EventListenerMouse* pMouseListener = EventListenerMouse::create();
	//pMouseListener->onMouseUp	
	getEventDispatcher()->addEventListenerWithFixedPriority(pKeyboardListener, 1);


	return m_baseSpriteFrameName.length() > 0;
}
void Player::initGraphics(const char* pathToXML)
{
	// Load the file
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError err = doc.LoadFile(Utils::appendFullPathToAssetsPath(pathToXML)
		.c_str());
	if (err)
	{
		cocos2d::log("Player: XML file not found: %s", pathToXML);
	}

	// Parse the file
	tinyxml2::XMLNode* pData = doc.RootElement();

	for (tinyxml2::XMLNode* pNode = pData->FirstChildElement(); pNode;
		pNode = pNode->NextSibling())
	{
		// If we found a walk animation
		std::string nodeValue(pNode->Value());
		if (nodeValue.compare(XML_ANIMATION_KEY) == 0)
		{
			tinyxml2::XMLNode* pPathToSpriteSheet = pNode->FirstChild();
			tinyxml2::XMLNode* pPathToPlistFile = pPathToSpriteSheet->NextSibling();
			tinyxml2::XMLNode* pSpriteFrameName = pPathToPlistFile->NextSibling();

			// Need to use full path to assets, because all configs are Assets folder relative
			m_walkSpritebatch = SpriteBatchNode::create(
				Utils::appendFullPathToAssetsPath(
					pPathToSpriteSheet->FirstChild()->ToText()->Value()));
			auto spriteCache = SpriteFrameCache::getInstance();
			spriteCache->addSpriteFramesWithFile(Utils::appendFullPathToAssetsPath(
				pPathToPlistFile->FirstChild()->ToText()->Value()));

			// Get template frame name
			const char* spriteFrameName = pSpriteFrameName->FirstChild()->
				ToText()->Value();

			// Get total frame count
			tinyxml2::XMLNode* pFrameCountNode = pSpriteFrameName->NextSibling();
			int frameCount = atoi(pFrameCountNode->FirstChild()->ToText()->Value());

			// Load all the frame for the animation
			char curSpriteFrameName[MAX_SPRITE_NAME_LENGTH] = { 0 };
			for (int i = 0; i < frameCount; i++)
			{
				sprintf(curSpriteFrameName, spriteFrameName, i);
				SpriteFrame* frame = spriteCache->getSpriteFrameByName(curSpriteFrameName);
				m_walkFrames.pushBack(frame);

				// Save first frame name of walk animation to use as Idle state
				if (i == 0)
				{
					m_baseSpriteFrameName = curSpriteFrameName;
				}
			}

			// init Sprite
			if (!initWithSpriteFrameName(m_baseSpriteFrameName))
			{
				cocos2d::log("Player: %s", "Sprite failed to initialize");
			}
		}
	}
}

void Player::update(float deltaTime)
{

}

void Player::onStartMoving(Vec2 direction)
{
	// Start character animation
	auto animation = Animation::createWithSpriteFrames(m_walkFrames, 0.1f);
	auto animate = Animate::create(animation);
	auto repeatAction = RepeatForever::create(animate);
	repeatAction->setTag(ACTION_ANIM_TAG);

	// remove any old anim action
	stopActionByTag(ACTION_ANIM_TAG);

	// start new anim
	runAction(repeatAction);
}

void Player::onCharacterMoveFinished()
{
	stopActionByTag(ACTION_ANIM_TAG);
	setSpriteFrame(SpriteFrameCache::getInstance()->
		getSpriteFrameByName(m_baseSpriteFrameName));
}

void Player::onKeyboardKeyUp(EventKeyboard::KeyCode keyCode, Event* pEvent)
{	
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_W:
		// Move up
		break;

	case EventKeyboard::KeyCode::KEY_S:
		// Move down
		break;

	case EventKeyboard::KeyCode::KEY_A:
		// Move left
		break;

	case EventKeyboard::KeyCode::KEY_D:
		// Move right
		break;

	case EventKeyboard::KeyCode::KEY_SPACE:
		// Perform dodge
		break;

	}
}