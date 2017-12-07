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
	// Default values
	m_moveSpeed = 0;
	m_dodgeSpeed = 0;
	m_moveDirection = Vec2::ZERO;

	Utils::initFromXML(*this, pathToXML);
	setAnchorPoint(Vec2(0.5, 0.5));

	Size size = Director::getInstance()->getWinSize();

	// Position player in middle of the sceen
	setPosition(size.width / 2, size.height / 2);

	// Register for input events	
	EventListenerKeyboard* pKeyboardListener = EventListenerKeyboard::create();
	pKeyboardListener->onKeyReleased = CC_CALLBACK_2(Player::onKeyboardKeyUp, this);
	pKeyboardListener->onKeyPressed = CC_CALLBACK_2(Player::onKeyboardKeyDown, this);
	
	EventListenerMouse* pMouseListener = EventListenerMouse::create();
	pMouseListener->onMouseUp = CC_CALLBACK_1(Player::onMouseButtonUp, this);	
	
	EventDispatcher* pEventDispatcher = getEventDispatcher();
	pEventDispatcher->addEventListenerWithFixedPriority(pKeyboardListener, 1);
	pEventDispatcher->addEventListenerWithFixedPriority(pMouseListener, 1);

	return true;
}

void Player::update(float deltaTime)
{
	setPosition(getPosition() + m_moveDirection * m_moveSpeed * deltaTime);
}

void Player::setMoveSpeed(float moveSpeed)
{
	m_moveSpeed = moveSpeed;
}

void Player::onKeyboardKeyUp(EventKeyboard::KeyCode keyCode, Event* pEvent)
{	
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_W:		
	case EventKeyboard::KeyCode::KEY_S:
		// Stop moving vertically
		m_moveDirection = Vec2(m_moveDirection.x, 0);
		break;

	case EventKeyboard::KeyCode::KEY_D:
	case EventKeyboard::KeyCode::KEY_A:
		// Stop moving horizontally
		m_moveDirection = Vec2(0, m_moveDirection.y);
		break;

	case EventKeyboard::KeyCode::KEY_SPACE:
		// Perform dodge
		m_bHasDodgeInput = true;
		break;
	}
}

void Player::onKeyboardKeyDown(EventKeyboard::KeyCode keyCode, Event* pEvent)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_W:
		// Move up
		m_moveDirection = Vec2(m_moveDirection.x, 1);
		break;

	case EventKeyboard::KeyCode::KEY_S:
		// Move down
		m_moveDirection = Vec2(m_moveDirection.x, -1);
		break;

	case EventKeyboard::KeyCode::KEY_A:
		// Move left
		m_moveDirection = Vec2(-1, m_moveDirection.y);
		break;

	case EventKeyboard::KeyCode::KEY_D:
		// Move right
		m_moveDirection = Vec2(1, m_moveDirection.y);
		break;
	}

	// Make sure we are not moving faster diagonally
	m_moveDirection.normalize();	
}

void Player::onMouseButtonUp(EventMouse* pEvent)
{
	EventMouse::MouseButton buttonUp = pEvent->getMouseButton();
	if(buttonUp == EventMouse::MouseButton::BUTTON_LEFT)
	{
		// Perform light attack
	}
	else if(buttonUp == EventMouse::MouseButton::BUTTON_RIGHT)
	{
		// Perform strong attack
	}
}