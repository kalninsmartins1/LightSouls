#include "Player.h"
#include "Utils/Utils.h"
#include "../Components/AnimComponent.h"
#include "Utils/XML/XMLLoader.h"
#include "GameConsts.h"
#include "Input/GameInput.h"
#include "physics3d/CCPhysics3DWorld.h"
#include "World/Physics/PhysicsManager.h"
#include "Events/HealthChangedEventData.h"
#include "cocos2d/cocos/base/CCEventDispatcher.h"
#include "Utils/AnimationUtils.h"

NS_LIGHTSOULS_BEGIN

const String Player::s_eventOnPlayerHealthChanged = "EVENT_ON_PLAYER_HEALTH_CHANGED";

Player* Player::Create(const String& pathToXML)
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

Player::Player()
	: m_animComponent(nullptr)
	, m_attackComponent(nullptr)
	, m_lastValidMoveDirection(Vector2::UNIT_X) // By default start out moving right
	, m_isAttackComboDelayExpired(true)
	, m_timeBetweenComboInput(0)
	, m_lastTimePerformedLightAttack(0)
	, m_curAttackAnimId(AnimationUtils::GetAnimId(ANIM_TYPE_ATTACK_COMBO_ONE))
	, m_lastAttackAnimId(AnimationUtils::GetAnimId(ANIM_TYPE_ATTACK_COMBO_FIVE))
	, m_firstAttackAnimId(m_curAttackAnimId)
{
}

const String& Player::GetOnHealthChangedEvent()
{
	return s_eventOnPlayerHealthChanged;
}

bool Player::Init(const String& pathToXML)
{
	XMLLoader::InitializeEntityUsingXMLFile(*this, pathToXML);	
	OnEntityInitialized();

	// Get animation component to trigger animations when that is necessary
	m_animComponent = static_cast<AnimComponent*>(getComponent(ANIM_COMPONENT));
	if (m_animComponent != nullptr)
	{
		m_animComponent->PlayLoopingAnimation(ANIM_TYPE_IDLE);
	}
	else
	{
		CCAssert(false, "Error: Did not find player animation component !");
	}
	m_attackComponent = static_cast<AttackComponent*>(getComponent(ATTACK_COMPONENT));

	SetPhysicsBodyAnchor(Vector2(0, 0));		
	PhysicsManager::GetInstance()->AddContactBeginListener(getName(), 
		CC_CALLBACK_1(Player::OnContactBegin, this));

	return true;
}

void Player::update(float deltaTime)
{
	// Call base update
	Entity::update(deltaTime);

	if (!IsDodging())
	{
		ManageInput();
	}

	// We can move only when we are not attacking
	if (!IsAttacking() && m_isAttackComboDelayExpired)
	{
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

	m_isAttackComboDelayExpired = false;
}

void Player::OnLightAttackComboExpired()
{
	m_isAttackComboDelayExpired = true;
}

void Player::ManageInput()
{
	GameInput* input = GameInput::GetInstance();
	if (input->HasAction("LightAttackInput"))
	{
		LightAttack();
	}
	else if (input->HasAction("StrongAttackInput"))
	{
		//lightAttack();
	}
	else if (input->HasAction("DodgeInput") && IsRunning())
	{
		PerformDodge();
	}

	// Player movement
	const float horizontalValue = input->GetInputAxis("HorizontalMovement");
	const float vertiacalValue = input->GetInputAxis("VerticalMovement");	
	Vector2& moveDirection = Vector2(horizontalValue, vertiacalValue);
	
	// Make sure we are not moving faster diagonally
	moveDirection.normalize();
	SetMoveDirection(moveDirection);

	// If we have an input, then update last valid move direction
	if (abs(horizontalValue) > MATH_EPSILON || abs(vertiacalValue) > MATH_EPSILON)
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

		if(!m_isAttackComboDelayExpired)
		{			
			// Wrap the index within valid values
			Utils::WrapValue(++m_curAttackAnimId, m_firstAttackAnimId, m_lastAttackAnimId);
		}
		else
		{
			// Reset back to first attack
			m_curAttackAnimId = m_firstAttackAnimId;				
		}
		
		// Play the attack animation
		m_animComponent->PlayOneShotAnimation(m_curAttackAnimId,
			CC_CALLBACK_0(Player::OnAttackFinished, this));
		m_attackComponent->Attack(m_lastValidMoveDirection);
		
		// Set the time last light attack was performed
		m_lastTimePerformedLightAttack = Utils::GetTimeStampInMilliseconds();
	}
}

void Player::PerformDodge()
{	
	StartDodging();
	m_animComponent->PlayLoopingAnimation(ANIM_TYPE_DODGE);
	Utils::StartTimerWithCallback(this,
		CC_CALLBACK_0(Player::OnDodgeFinished, this), GetDodgeTime());
}

void Player::PlayRunOrIdleAnimation() const
{			
	if (IsRunning() && !IsDodging())
	{
		if(!m_animComponent->IsCurrrentlyPlayingAnimation(ANIM_TYPE_RUN))
		{
			m_animComponent->PlayLoopingAnimation(ANIM_TYPE_RUN);
		}
	}
	else if (!IsRunning())
	{
		if(!m_animComponent->IsCurrrentlyPlayingAnimation(ANIM_TYPE_IDLE))
		{
			m_animComponent->PlayLoopingAnimation(ANIM_TYPE_IDLE);
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
	const String& name = otherBody->getNode()->getName();
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

NS_LIGHTSOULS_END
