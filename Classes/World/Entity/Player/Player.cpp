#include "Player.h"
#include "Utils/Utils.h"
#include "../Components/AnimComponent.h"
#include "Utils/XML/XMLLoader.h"
#include "GameConsts.h"
#include "Input/GameInput.h"
#include "physics3d/CCPhysics3DWorld.h"
#include "World/Physics/PhysicsManager.h"
#include "Events/ProgressBarChangedEventData.h"
#include "cocos2d/cocos/base/CCEventDispatcher.h"
#include "Utils/AnimationUtils.h"

NS_LIGHTSOULS_BEGIN

const String Player::s_eventOnPlayerHealthChanged = "EVENT_ON_PLAYER_HEALTH_CHANGED";
const String Player::s_eventOnPlayerStaminaChanged = "EVENT_ON_PLAYER_STAMINA_CHANGED";

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
	: m_attackComponent(nullptr)
	, m_lastValidMoveDirection(Vector2::UNIT_X) // By default start out moving right
	, m_isDodging(false)
	, m_isAttackComboDelayExpired(true)
	, m_dodgeSpeed(0.0f)
	, m_dodgeTime(0.0f)
	, m_timeBetweenComboInput(0.0f)
	, m_dodgeStaminaConsumption(0.0f)
	, m_lastTimePerformedLightAttack(0.0f)
	, m_curAttackAnimId(AnimationUtils::GetAnimId(ANIM_TYPE_ATTACK_COMBO_ONE))
	, m_lastAttackAnimId(AnimationUtils::GetAnimId(ANIM_TYPE_ATTACK_COMBO_FIVE))
	, m_firstAttackAnimId(m_curAttackAnimId)
{
}

const String& Player::GetEventOnHealthChanged()
{
	return s_eventOnPlayerHealthChanged;
}

const String& Player::GetEventOnStaminaChanged()
{
	return s_eventOnPlayerStaminaChanged;
}

bool Player::Init(const String& pathToXML)
{
	XMLLoader::InitializeEntityUsingXMLFile(*this, pathToXML);
	
	OnEntityInitialized();
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

	// We can move only when we are not attacking
	if (IsReadyToAttack() && !m_isDodging)
	{
		ManageInput();
	}

	// If still not attacking
	if (IsReadyToAttack())
	{
		PlayRunOrIdleAnimation();
	}
}

void Player::SetTimeBetweenComboInput(float timeBetweenComboInput)
{
	m_timeBetweenComboInput = timeBetweenComboInput;
}

void Player::SetDodgeStaminaConsumption(float dodgeStaminaConumption)
{
	m_dodgeStaminaConsumption = dodgeStaminaConumption;
}

void Player::SetDodgeSpeed(float dodgeSpeed)
{
	m_dodgeSpeed = dodgeSpeed;
}

void Player::SetDodgeTime(float dodgeTime)
{
	m_dodgeTime = dodgeTime;
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
	else if (input->HasAction("DodgeInput") && IsRunning() && 
		HasEnoughtStamina(m_dodgeStaminaConsumption))
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

void Player::StartDodging()
{
	m_isDodging = true;
	SetCurrentMoveSpeed(m_dodgeSpeed);

	// Consume stamina
	ConsumeStamina(m_dodgeStaminaConsumption);
}

void Player::StopDodging()
{
	m_isDodging = false;
	ResetMoveSpeed();
}

void Player::LightAttack()
{	
	if (IsReadyToAttack() && !m_isDodging && m_attackComponent->IsReadyToAttack())
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
		GetAnimComponent()->PlayOneShotAnimation(m_curAttackAnimId,
			CC_CALLBACK_0(Player::OnAttackFinished, this));
		m_attackComponent->Attack(m_lastValidMoveDirection);
		
		// Set the time last light attack was performed
		m_lastTimePerformedLightAttack = Utils::GetTimeStampInMilliseconds();
	}
}

void Player::PerformDodge()
{	
	StartDodging();
	GetAnimComponent()->PlayLoopingAnimation(ANIM_TYPE_DODGE);
	Utils::StartTimerWithCallback(this,
		CC_CALLBACK_0(Player::OnDodgeFinished, this), m_dodgeTime);
}

void Player::PlayRunOrIdleAnimation() const
{		
	auto animComponent = GetAnimComponent();
	if (animComponent != nullptr)
	{
		if (IsRunning() && !m_isDodging)
		{
			if (!animComponent->IsCurrrentlyPlayingAnimation(ANIM_TYPE_RUN))
			{
				animComponent->PlayLoopingAnimation(ANIM_TYPE_RUN);
			}
		}
		else if (!IsRunning())
		{
			if (!animComponent->IsCurrrentlyPlayingAnimation(ANIM_TYPE_IDLE))
			{
				animComponent->PlayLoopingAnimation(ANIM_TYPE_IDLE);
			}
		}
	}
}

void Player::DispatchOnHealthChangedEvent()
{
	float currentHealth = GetCurrentHealth();
	float healthPercentage = Utils::SafeDevide(currentHealth, GetMaxHealth());
	auto eventData = ProgressBarChangedEventData(GetId(), currentHealth, healthPercentage);
	getEventDispatcher()->dispatchCustomEvent(s_eventOnPlayerHealthChanged,
		&eventData);
}

void Player::DispatchOnStaminaChangedEvent()
{
	float currentStamina = GetCurrentStamina();
	float staminaPercentage = Utils::SafeDevide(currentStamina, GetMaxStamina());
	auto eventData = ProgressBarChangedEventData(GetId(), currentStamina, staminaPercentage);
	getEventDispatcher()->dispatchCustomEvent(s_eventOnPlayerStaminaChanged,
		&eventData);
}

void Player::OnContactBegin(const cocos2d::PhysicsBody* otherBody)
{
// 	const String& name = otherBody->getNode()->getName();
// 	CCLOG("Player collided with %s", name.c_str());
}

float Player::GetSecondsForValidLighAttackCombo() const
{
	const long currnentTime = Utils::GetTimeStampInMilliseconds();
	const float secondsSinceLastAttack = // Divide by 1000 to convert to seconds
		(currnentTime - m_lastTimePerformedLightAttack) / 1000.f;

	const float secondsBeforeComboExpires = m_timeBetweenComboInput - secondsSinceLastAttack;

	return secondsBeforeComboExpires;
}

NS_LIGHTSOULS_END
