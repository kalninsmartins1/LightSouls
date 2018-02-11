#include "Player.h"
#include "Utils/Utils.h"
#include "../Components/PlayerAnimComponent.h"
#include "Utils/XML/XMLLoader.h"
#include "GameConsts.h"
#include "Input/GameInput.h"
#include "physics3d/CCPhysics3DWorld.h"

using namespace cocos2d;

Player* Player::create(const std::string& pathToXML)
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
	m_bIsAttackComboDelayExpired(true),
	m_timeBetweenComboInput(0),	
	m_lastTimePerformedLightAttack(0),
	m_curLightAttackAnimIdx(0)
{
}

bool Player::init(const std::string& pathToXML)
{
	XMLLoader::initializeSpriteUsingXMLFile(*this, pathToXML);

	// Position physics body at the bottom of sprite
	getPhysicsBody()->setPositionOffset(Vec2(0, -getContentSize().height / 2));

	// Force position player in middle of screen
	const Size size = Director::getInstance()->getWinSize();
	setPosition(size.width / 2, size.height / 2);

	// Get animation component to trigger animations when that is necessary
	m_pPlayerAnimComponent =
		dynamic_cast<PlayerAnimComponent*>(
			getComponent(PLAYER_ANIM_COMPONENT));
	m_pPlayerAnimComponent->loopIdleAnimation();

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
	if (!m_bIsAttacking && m_bIsAttackComboDelayExpired)
	{
		setPosition(getPosition() + m_moveDirection *
			m_moveSpeed * deltaTime);
		m_bIsRuning = m_moveDirection.lengthSquared() > 0;
		playRunOrIdleAnimation();		
	}	
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
}

void Player::onAttackFinished()
{
	m_bIsAttacking = false;
	// Start light attack combo expiration
	const float secondsBeforeComboInputExpires = getSecondsForValidLighAttackCombo();
	Utils::startTimerWithCallback(this, 
		CC_CALLBACK_0(Player::onLightAttackComboExpired, this),
		secondsBeforeComboInputExpires);

	m_bIsAttackComboDelayExpired = false;
}

void Player::onLightAttackComboExpired()
{
	m_bIsAttackComboDelayExpired = true;
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
	else if (pInput->hasAction("DodgeInput") && m_bIsRuning)
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

		if(!m_bIsAttackComboDelayExpired)
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
	m_pPlayerAnimComponent->loopDodgeAnimation();
	Utils::startTimerWithCallback(this,
		CC_CALLBACK_0(Player::onDodgeFinished, this), m_dodgeTime);
}

void Player::playRunOrIdleAnimation() const
{			
	if (m_bIsRuning && !m_bIsDodging)
	{
		if(m_pPlayerAnimComponent->getCurrentlyLoopingAnimation() 
			!= AnimationKind::RUN)
		{
			m_pPlayerAnimComponent->loopRunAnimation();
		}
	}
	else if (!m_bIsRuning)
	{
		if(m_pPlayerAnimComponent->getCurrentlyLoopingAnimation() 
			!= AnimationKind::IDLE)
		{
			m_pPlayerAnimComponent->loopIdleAnimation();
		}
	}
}

float Player::getSecondsForValidLighAttackCombo() const
{
	const long currnentTime = Utils::getTimeStampInMilliseconds();
	const float secondsSinceLastAttack = // Devide by 1000 to convert to seconds
		(currnentTime - m_lastTimePerformedLightAttack) / 1000.f;

	const float secondsBeforeComboExpires = m_timeBetweenComboInput - secondsSinceLastAttack;

	return secondsBeforeComboExpires;
}
