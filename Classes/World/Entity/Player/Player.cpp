#include "Player.h"
#include "Utils/Utils.h"
#include "../Components/PlayerAnimComponent.h"
#include "Utils/XML/XMLLoader.h"
#include "GameConsts.h"
#include "Input/GameInput.h"
#include "physics3d/CCPhysics3DWorld.h"
#include "World/Physics/PhysicsManager.h"
#include "Events/HealthChangedEventData.h"
#include "cocos2d/cocos/base/CCEventDispatcher.h"

using namespace cocos2d;

const std::string Player::s_eventOnPlayerHealthChanged = "EVENT_ON_PLAYER_HEALTH_CHANGED";

Player* Player::Create(const std::string& pathToXML)
{
	Player* player = new (std::nothrow) Player();
	if (player && player->Init(pathToXML))
	{
		player->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(player);
	}

	return player;
}

Player::Player() :
	m_animComponent(nullptr),
	m_attackComponent(nullptr),
	m_lastValidMoveDirection(Vector2::UNIT_X), // By default start out moving right
	m_bIsAttackComboDelayExpired(true),
	m_timeBetweenComboInput(0),	
	m_lastTimePerformedLightAttack(0),
	m_curLightAttackAnimIdx(0)
{
}

const std::string& Player::GetOnHealthChangedEvent()
{
	return s_eventOnPlayerHealthChanged;
}

bool Player::Init(const std::string& pathToXML)
{
	XMLLoader::InitializeEntityUsingXMLFile(*this, pathToXML);

	// Force position player in middle of screen
	const Size size = Director::getInstance()->getWinSize();
	setPosition(size.width / 2, size.height / 2);

	// Get animation component to trigger animations when that is necessary
	m_animComponent =
		static_cast<PlayerAnimComponent*>(getComponent(PLAYER_ANIM_COMPONENT));
	m_animComponent->loopIdleAnimation();

	m_attackComponent = static_cast<AttackComponent*>(getComponent(ATTACK_COMPONENT));
	
	PhysicsManager::GetInstance()->AddContactListener(getName(), 
		CC_CALLBACK_1(Player::OnContactBegin, this));

	return true;
}

void Player::update(float deltaTime)
{
	// Call base update
	Entity::update(deltaTime);

	ManageInput();

	// We can move only when we are not attacking
	if (!IsAttacking() && m_bIsAttackComboDelayExpired)
	{
		setPosition(getPosition() + GetHeading() *
			GetCurrentMoveSpeed() * deltaTime);
		
		PlayRunOrIdleAnimation();
	}	
}

void Player::SetTimeBetweenComboInput(float timeBetweenComboInput)
{
	m_timeBetweenComboInput = timeBetweenComboInput;
}

void Player::OnDodgeFinished()
{
	StopDodging();
}

void Player::OnAttackFinished()
{
	StopAttacking();

	// Start light attack combo expiration
	const float secondsBeforeComboInputExpires = GetSecondsForValidLighAttackCombo();
	Utils::StartTimerWithCallback(this, 
		CC_CALLBACK_0(Player::OnLightAttackComboExpired, this),
		secondsBeforeComboInputExpires);

	m_bIsAttackComboDelayExpired = false;
}

void Player::OnLightAttackComboExpired()
{
	m_bIsAttackComboDelayExpired = true;
}

void Player::ManageInput()
{
	GameInput* pInput = GameInput::GetInstance();
	if (pInput->hasAction("LightAttackInput"))
	{
		LightAttack();
	}
	else if (pInput->hasAction("StrongAttackInput"))
	{
		//lightAttack();
	}
	else if (pInput->hasAction("DodgeInput") && IsRunning())
	{
		PerformDodge();
	}

	// Player movement
	const float horizontalValue = pInput->getInputAxis("HorizontalMovement");
//	const float vertiacalValue = pInput->getInputAxis("VerticalMovement");	
	Vector2& moveDirection = Vector2(horizontalValue, 0.0f);
	
	// Make sure we are not moving faster diagonally
	moveDirection.normalize();
	SetMoveDirection(moveDirection);

	// If we have an input, then update last valid move direction
	if (abs(horizontalValue) > MATH_EPSILON)//|| abs(vertiacalValue) > MATH_EPSILON)
	{
		m_lastValidMoveDirection = moveDirection;
	}
}

void Player::LightAttack()
{	
	if (!IsAttacking() && !IsDodging())
	{
		// Activating attack
		Entity::StartAttacking();

		if(!m_bIsAttackComboDelayExpired)
		{
			// Go to next attack in combo
			m_curLightAttackAnimIdx++;
			
			// Wrap the index within valid values
			Utils::WrapValue(m_curLightAttackAnimIdx, 
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
		m_animComponent->playLightAttackAnimation(
			static_cast<LightAttackStage>(m_curLightAttackAnimIdx),
			CC_CALLBACK_0(Player::OnAttackFinished, this));
		m_attackComponent->Attack(m_lastValidMoveDirection);
		
		// Set the time last light attack was performed
		m_lastTimePerformedLightAttack = Utils::GetTimeStampInMilliseconds();
	}
}

void Player::PerformDodge()
{	
	StartDodging();
	m_animComponent->loopDodgeAnimation();
	Utils::StartTimerWithCallback(this,
		CC_CALLBACK_0(Player::OnDodgeFinished, this), GetDodgeTime());
}

void Player::PlayRunOrIdleAnimation() const
{			
	if (IsRunning() && !IsDodging())
	{
		if(m_animComponent->getCurrentlyLoopingAnimation() 
			!= AnimationKind::RUN)
		{
			m_animComponent->loopRunAnimation();
		}
	}
	else if (!IsRunning())
	{
		if(m_animComponent->getCurrentlyLoopingAnimation() 
			!= AnimationKind::IDLE)
		{
			m_animComponent->loopIdleAnimation();
		}
	}
}

void Player::DispatchOnHealthChangedEvent()
{
	float currentHealth = GetCurrentHealth();
	float healthPercentage = Utils::SafeDevide(currentHealth, GetMaxHealth());
	auto eventData = HealthChangedEventData(GetId(), currentHealth, healthPercentage);
	getEventDispatcher()->dispatchCustomEvent(s_eventOnPlayerHealthChanged,
		&eventData);

	CCLOG("Player health changed %f", GetCurrentHealth());
}

void Player::OnContactBegin(const cocos2d::PhysicsBody* otherBody)
{
	const std::string& name = otherBody->getNode()->getName();
	CCLOG("Player collided with %s", name.c_str());
}

float Player::GetSecondsForValidLighAttackCombo() const
{
	const long currnentTime = Utils::GetTimeStampInMilliseconds();
	const float secondsSinceLastAttack = // Devide by 1000 to convert to seconds
		(currnentTime - m_lastTimePerformedLightAttack) / 1000.f;

	const float secondsBeforeComboExpires = m_timeBetweenComboInput - secondsSinceLastAttack;

	return secondsBeforeComboExpires;
}
