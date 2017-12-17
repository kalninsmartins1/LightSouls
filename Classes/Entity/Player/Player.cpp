#include "Player.h"
#include "Utils/Utils.h"
#include "GameConsts.h"
#include "Entity/Components/PlayerAnimComponent.h"
#include "Utils/XMLLoader.h"
#include "Input/GameInput.h"

using namespace cocos2d;

Player* Player::create(const char* pathToXML)
{
	Player* pPlayer = new (std::nothrow) Player();
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
	// Set default values
	m_moveDirection = Vec2::ZERO;
	m_baseMoveSpeed = 0;
	m_moveSpeed = 0;
	m_dodgeSpeed = 0;
	m_dodgeTime = 0;
	m_bIsAttacking = false;

	XMLLoader::initializeSpriteUsingXMLFile(*this, pathToXML);	

	// Position physics body at the bottom of sprite
	getPhysicsBody()->setPositionOffset(Vec2(0, -getContentSize().height/2));

	// Force position player in middle of screen
	Size size = Director::getInstance()->getWinSize();	
	setPosition(size.width / 2 , size.height / 2);

	// Register for input events	
	EventListenerKeyboard* pKeyboardListener = EventListenerKeyboard::create();
	pKeyboardListener->onKeyReleased = CC_CALLBACK_2(Player::onKeyboardKeyUp, this);
	pKeyboardListener->onKeyPressed = CC_CALLBACK_2(Player::onKeyboardKeyDown, this);
	
	EventListenerMouse* pMouseListener = EventListenerMouse::create();
	pMouseListener->onMouseUp = CC_CALLBACK_1(Player::onMouseButtonUp, this);	
	
	EventDispatcher* pEventDispatcher = getEventDispatcher();
	pEventDispatcher->addEventListenerWithFixedPriority(pKeyboardListener, 1);
	pEventDispatcher->addEventListenerWithFixedPriority(pMouseListener, 1);

	// Get animation component to trigger animations when that is necessary
	m_pPlayerAnimComponent = 
		(PlayerAnimComponent*)getComponent(XML_PLAYER_ANIM_COMPONENT);
	m_pPlayerAnimComponent->startAnimation(PlayerAnimationType::Idle);

	// Set move speed at begining
	m_moveSpeed = m_baseMoveSpeed;

	return true;
}

void Player::update(float deltaTime)
{
	// Call base update
	Sprite::update(deltaTime);

	// We can move only when we are not attacking
	if(!m_bIsAttacking)
	{
		setPosition(getPosition() + m_moveDirection * m_moveSpeed * deltaTime);
	}	

	GameInput* pInput = GameInput::getInstance();
	if(pInput->HasActionInput("LightAttackInput"))
	{
		
	}
	else if(pInput->HasActionInput("StrongAttackInput"))
	{
		
	}
	else if(pInput->HasActionInput("DodgeInput"))
	{
		
	}

}

void Player::setMoveSpeed(float moveSpeed)
{
	m_baseMoveSpeed = moveSpeed;
}

void Player::setDodgeSpeed(float dodgeSpeed)
{
	m_dodgeSpeed = dodgeSpeed;
}

void Player::setDodgeTime(float dodgeTime)
{
	m_dodgeTime = dodgeTime;
}

Vec2 Player::getHeading()
{
	return m_moveDirection;
}

void Player::onKeyboardKeyUp(EventKeyboard::KeyCode keyCode, Event* pEvent)
{		
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_W:		
		// Stop moving up
		if(m_moveDirection.y > 0)
		{
			m_moveDirection = Vec2(m_moveDirection.x, 0);
		}		
		break;

	case EventKeyboard::KeyCode::KEY_S:
		// Stop moving down
		if(m_moveDirection.y < 0)
		{
			m_moveDirection = Vec2(m_moveDirection.x, 0);
		}		
		break;

	case EventKeyboard::KeyCode::KEY_D:
		// Stop moving right
		if(m_moveDirection.x > 0)
		{
			m_moveDirection = Vec2(0, m_moveDirection.y);
		}		
		break;

	case EventKeyboard::KeyCode::KEY_A:
		// Stop moving left
		if(m_moveDirection.x < 0)
		{
			m_moveDirection = Vec2(0, m_moveDirection.y);
		}		
		break;

	case EventKeyboard::KeyCode::KEY_SPACE:
		// Dodge if currently not dodging
		if(!m_bIsDodging)
		{
			PerformDodge();
		}		
		break;
	}

	// If we dont have move direction then we must be standing
	if(m_moveDirection.lengthSquared() == 0)
	{
		m_pPlayerAnimComponent->startAnimation(PlayerAnimationType::Idle);
		m_bIsMoving = false;
	}
	else
	{
		m_bIsMoving = true;
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

	if(!m_bIsDodging && m_moveDirection.lengthSquared() > 0 && !m_bIsAttacking)
	{
		// When keyboard is down we are always moving
		m_pPlayerAnimComponent->startAnimation(PlayerAnimationType::Run);
		m_bIsMoving = true;
	}		
}

void Player::onMouseButtonUp(EventMouse* pEvent)
{
	/*EventMouse::MouseButton buttonUp = pEvent->getMouseButton();
	if(buttonUp == EventMouse::MouseButton::BUTTON_LEFT)
	{*/
	if(!m_bIsAttacking && !m_bIsDodging)
	{
		// Perform light attack
		m_bIsAttacking = true;
		m_pPlayerAnimComponent->startAnimation(PlayerAnimationType::Attack);
		Utils::startTimerWithCallback(this,
			CC_CALLBACK_0(Player::onAttackFinished, this),
			m_pPlayerAnimComponent->
			getAnimationLengthInSeconds(PlayerAnimationType::Attack));
	}
		
	/*}
	else if(buttonUp == EventMouse::MouseButton::BUTTON_RIGHT)
	{
		// Perform strong attack
		m_pPlayerAnimComponent->startAnimation(PlayerAnimationType::Attack);
	}*/
}

void Player::onDodgeFinished()
{
	m_bIsDodging = false;

	// Set back regular speed
	m_moveSpeed = m_baseMoveSpeed;	

	if(m_bIsMoving)
	{
		// Go back to regular running
		m_pPlayerAnimComponent->startAnimation(PlayerAnimationType::Run);
	}
}

void Player::onAttackFinished()
{
	m_bIsAttacking = false;

	if(m_bIsMoving)
	{
		m_pPlayerAnimComponent->startAnimation(PlayerAnimationType::Run);
	}
	else
	{
		m_pPlayerAnimComponent->startAnimation(PlayerAnimationType::Idle);
	}
}

void Player::PerformDodge()
{
	m_bIsDodging = true;
	m_moveSpeed = m_dodgeSpeed;
	m_pPlayerAnimComponent->startAnimation(PlayerAnimationType::Dodge);
	Utils::startTimerWithCallback(this,
		CC_CALLBACK_0(Player::onDodgeFinished, this), m_dodgeTime);
}
