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

Player::Player() :
	
	m_pPlayerAnimComponent(nullptr),
	m_moveDirection(Vec2::ZERO),
	m_isRuning(false),
	m_bIsDodging(false),
	m_bIsAttacking(false),
	m_baseMoveSpeed(0),
	m_moveSpeed(0),
	m_dodgeSpeed(0),
	m_dodgeTime(0),	
	m_timeBetweenComboInput(0),
	m_curTimeBetweenComboInput(0),
	m_lastTimePerformedLightAttack(0),
	m_curLightAttackAnimIdx(static_cast<unsigned short int>(LightAttackStage::ONE))
{
}

bool Player::init(const char* pathToXML)
{
	XMLLoader::initializeSpriteUsingXMLFile(*this, pathToXML);

	// Position physics body at the bottom of sprite
	getPhysicsBody()->setPositionOffset(Vec2(0, -getContentSize().height / 2));

	// Force position player in middle of screen
	Size size = Director::getInstance()->getWinSize();
	setPosition(size.width / 2, size.height / 2);

	// Get animation component to trigger animations when that is necessary
	m_pPlayerAnimComponent =
		dynamic_cast<PlayerAnimComponent*>(getComponent(XML_PLAYER_ANIM_COMPONENT));
	m_pPlayerAnimComponent->playIdleAnimation();

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
	if (!m_bIsAttacking)
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

Vec2 Player::getHeading() const
{
	return m_moveDirection;
}

void Player::setTimeBetweenComboInput(float timeBetweenComboInput)
{
	m_timeBetweenComboInput = timeBetweenComboInput;
}

void Player::onDodgeFinished()
{
	m_bIsDodging = false;

	// Set back regular speed
	m_moveSpeed = m_baseMoveSpeed;

	if (m_isRuning)
	{
		// Go back to regular running
		m_pPlayerAnimComponent->playRunAnimation();
	}
}

void Player::onAttackFinished()
{
	m_bIsAttacking = false;

	if (m_isRuning)
	{
		m_pPlayerAnimComponent->playRunAnimation();
	}
	else
	{
		m_pPlayerAnimComponent->playIdleAnimation();
	}
}

void Player::manageInput()
{
	GameInput* pInput = GameInput::getInstance();
	if (pInput->hasAction("LightAttackInput"))
	{
		lightAttack();
	}
	else if (pInput->hasAction("StrongAttackInput"))
	{
		//lightAttack();
	}
	else if (pInput->hasAction("DodgeInput") && m_isRuning)
	{
		performDodge();
	}

	// Player movement
	const float horizontalValue = pInput->getInputAxis("HorizontalMovement");
	const float vertiacalValue = pInput->getInputAxis("VerticalMovement");
	m_moveDirection = Vec2(horizontalValue, vertiacalValue);

	// Make sure we are not moving faster diagonally
	m_moveDirection.normalize();
}

void Player::lightAttack()
{
	if (!m_bIsAttacking && !m_bIsDodging)
	{
		// Activating attack
		m_bIsAttacking = true;
		
		const long currnentTime = Utils::getTimeStampInMilliseconds();
		const float secondsSinceLastAttack = // Devide by 1000 to convert to seconds
			(currnentTime - m_lastTimePerformedLightAttack) / 1000.f;
		CCLOG("Time since last light attack %f", secondsSinceLastAttack);
		const bool bIsComboActive = secondsSinceLastAttack < m_timeBetweenComboInput;
		
		if(bIsComboActive)
		{
			// Go to next attack in combo
			m_curLightAttackAnimIdx++;
			
			// Wrap the index within valid values
			Utils::wrapValue(m_curLightAttackAnimIdx, 
				static_cast<unsigned short int>(LightAttackStage::ONE),
				static_cast<unsigned short int>(LightAttackStage::FIVE));
		}
		else
		{
			// Reset back to first attack
			m_curLightAttackAnimIdx = 
				static_cast<unsigned short int>(LightAttackStage::ONE);
		}
		
		// Play the attack animation
		m_pPlayerAnimComponent->playLightAttackAnimation(
			static_cast<LightAttackStage>(m_curLightAttackAnimIdx),
			CC_CALLBACK_0(Player::onAttackFinished, this));
		
		// Set the time last light attack was performed
		m_lastTimePerformedLightAttack = Utils::getTimeStampInMilliseconds();
	}
}

void Player::performDodge()
{
	m_bIsDodging = true;
	m_moveSpeed = m_dodgeSpeed;
	m_pPlayerAnimComponent->playDodgeAnimation();
	Utils::startTimerWithCallback(this,
		CC_CALLBACK_0(Player::onDodgeFinished, this), m_dodgeTime);
}

void Player::playAnimations()
{
	if (!m_bIsAttacking)
	{
		const float moveDirectionSqrt = m_moveDirection.lengthSquared();
		if (!m_bIsDodging && moveDirectionSqrt > 0 && !m_isRuning)
		{
			// When keyboard is down we are always moving
			m_pPlayerAnimComponent->playRunAnimation();
			m_isRuning = true;
		}
		else if (moveDirectionSqrt <= FLT_EPSILON && m_isRuning)
		{
			m_pPlayerAnimComponent->playIdleAnimation();
			m_isRuning = false;
		}
	}
}
