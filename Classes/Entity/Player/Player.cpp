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

Player::Player()
{
	// Set default values
	m_moveDirection = Vec2::ZERO;
	m_baseMoveSpeed = 0;
	m_moveSpeed = 0;
	m_dodgeSpeed = 0;
	m_dodgeTime = 0;
	m_bIsAttacking = false;
	m_bIsDodging = false;
	m_bIsRunAnimPlaying = false;	
}

bool Player::init(const char* pathToXML)
{	
	XMLLoader::initializeSpriteUsingXMLFile(*this, pathToXML);	

	// Position physics body at the bottom of sprite
	getPhysicsBody()->setPositionOffset(Vec2(0, -getContentSize().height/2));

	// Force position player in middle of screen
	Size size = Director::getInstance()->getWinSize();	
	setPosition(size.width / 2 , size.height / 2);

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

	manageInput();

	// We can move only when we are not attacking
	if(!m_bIsAttacking)
	{
		setPosition(getPosition() + m_moveDirection * m_moveSpeed * deltaTime);
	}
	
	playAnimations();
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

void Player::onDodgeFinished()
{
	m_bIsDodging = false;

	// Set back regular speed
	m_moveSpeed = m_baseMoveSpeed;	

	if(m_bIsRunAnimPlaying)
	{
		// Go back to regular running
		m_pPlayerAnimComponent->startAnimation(PlayerAnimationType::Run);
	}
}

void Player::onAttackFinished()
{
	m_bIsAttacking = false;

	if(m_bIsRunAnimPlaying)
	{
		m_pPlayerAnimComponent->startAnimation(PlayerAnimationType::Run);
	}
	else
	{
		m_pPlayerAnimComponent->startAnimation(PlayerAnimationType::Idle);
	}
}

void Player::manageInput()
{
	GameInput* pInput = GameInput::getInstance();
	if(pInput->hasAction("LightAttackInput"))
	{
		lightAttack();
	}
	else if(pInput->hasAction("StrongAttackInput"))
	{
		lightAttack();
	}
	else if(pInput->hasAction("DodgeInput"))
	{
		performDodge();
	}
	
	// Player movement
	float horizontalValue = pInput->getInputAxis("HorizontalMovement");
	float vertiacalValue = pInput->getInputAxis("VerticalMovement");
	m_moveDirection = Vec2(horizontalValue, vertiacalValue);	

	// Make sure we are not moving faster diagonally
	m_moveDirection.normalize();	
}

void Player::lightAttack()
{
	if (!m_bIsAttacking && !m_bIsDodging)
	{
		// Perform light attack
		m_bIsAttacking = true;
		m_pPlayerAnimComponent->startAnimation(PlayerAnimationType::Attack);
		Utils::startTimerWithCallback(this,
			CC_CALLBACK_0(Player::onAttackFinished, this),
			m_pPlayerAnimComponent->
			getAnimationLengthInSeconds(PlayerAnimationType::Attack));
	}
}

void Player::performDodge()
{
	m_bIsDodging = true;
	m_moveSpeed = m_dodgeSpeed;
	m_pPlayerAnimComponent->startAnimation(PlayerAnimationType::Dodge);
	Utils::startTimerWithCallback(this,
		CC_CALLBACK_0(Player::onDodgeFinished, this), m_dodgeTime);
}

void Player::playAnimations()
{
	if(!m_bIsAttacking)
	{
		float moveDirectionSqrt = m_moveDirection.lengthSquared();
		if(!m_bIsDodging && moveDirectionSqrt > 0 && !m_bIsRunAnimPlaying)
		{
			// When keyboard is down we are always moving
			m_pPlayerAnimComponent->startAnimation(PlayerAnimationType::Run);
			m_bIsRunAnimPlaying = true;
		}	
		else if(moveDirectionSqrt <= FLT_EPSILON && m_bIsRunAnimPlaying)
		{
			m_pPlayerAnimComponent->startAnimation(PlayerAnimationType::Idle);
			m_bIsRunAnimPlaying = false;		
		}
	}
}
