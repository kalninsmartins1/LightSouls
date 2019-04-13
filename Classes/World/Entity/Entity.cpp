#include "Entity.h"
#include "World/Entity/Components/AnimComponent.h"
#include "GameConsts.h"
#include "Utils/Utils.h"
#include "Classes/Utils/AnimationUtils.h"

unsigned int Entity::s_uniqueId = 0;

Entity::Entity()
	: m_id(s_uniqueId++)
	, m_animComponent(nullptr)
	, m_moveDirection(Vector2::ZERO)
	, m_physicsBodyScaledSize(cocos2d::Size::ZERO)
	, m_isRuning(false)
	, m_isAttacking(false)
	, m_isTakingDamage(false)
	, m_isDisappearing(false)
	, m_isStaminaRegenerateDelayExpired(true)
	, m_baseMoveSpeed(0.0f)
	, m_baseHealth(0.0f)
	, m_baseDamage(0.0f)
	, m_baseStamina(0.0f)
	, m_stamina(0.0f)
	, m_health(0.0f)
	, m_damage(0.0f)
	, m_moveSpeed(0.0f)
	, m_physicsBodyForceScale(1.0f)
	, m_staminaRegenerateSpeed(0.0f)
	, m_staminaRegenerateDelay(0.0f)
	, m_knockBackStrenght(0.0f)	
{
	
}

Entity::~Entity()
{

}

EntityType Entity::GetEntityType() const
{
	return EntityType::NONE;
}

void Entity::UpdateSortingLayer()
{
	// Invert the y since higher y values should be closer
	setLocalZOrder(-_position.y);
}

void Entity::SetBaseDamage(float baseDamage)
{
	m_baseDamage = baseDamage;
	m_damage = baseDamage;
}

void Entity::SetCurrentMoveSpeed(float moveSpeed)
{
	m_moveSpeed = moveSpeed;
	if (_physicsBody != nullptr)
	{
		_physicsBody->setVelocityLimit(moveSpeed);
	}	
}

void Entity::SetStaminaRegenerateSpeed(float regenerateSpeed)
{
	m_staminaRegenerateSpeed = regenerateSpeed;
}

void Entity::SetStaminaRegenerateDelay(float regenerateDelay)
{
	m_staminaRegenerateDelay = regenerateDelay;
}

void Entity::SetKnockBackStrenght(float strenght)
{
	m_knockBackStrenght = strenght;
}

void Entity::SetTimeModifier(float timeModifier)
{
	setTimeModifier(timeModifier);
}

void Entity::setScale(float scaleX, float scaleY)
{
	Sprite::setScale(scaleX, scaleY);
	float entityScale = Utils::Avg(scaleX, scaleY);
	SetBaseMoveSpeed(m_baseMoveSpeed * entityScale * Utils::SafeDevide(1.0f, Utils::GetScaleFactor()));
}

void Entity::ResetHealth()
{
	m_health = m_baseHealth;
}

void Entity::ResetStamina()
{
	m_stamina = m_baseStamina;
}

void Entity::SetBaseHealth(float baseHealth)
{
	m_baseHealth = baseHealth;
	m_health = baseHealth;
}

void Entity::SetBaseStamina(float baseStamina)
{
	m_baseStamina = baseStamina;
	m_stamina = baseStamina;
}

void Entity::SetBaseMoveSpeed(float moveSpeed)
{
	m_baseMoveSpeed = moveSpeed;
	SetCurrentMoveSpeed(moveSpeed);
}

void Entity::SetMoveDirection(const Vector2& direction)
{
	m_moveDirection = direction;
}

void Entity::SetPhysicsBodySize(const cocos2d::Size& size)
{
	const float scaleFactor = Utils::GetScaleFactor();
	m_physicsBodyScaledSize.width = size.width * abs(getScaleX()) * scaleFactor;
	m_physicsBodyScaledSize.height = size.height * abs(getScaleY()) * scaleFactor;
}

void Entity::SetPhysicsBodyAnchor(const Vector2& achorPos)
{
	const cocos2d::Size& size = GetPhysicsBodySizeScaled();
	float physicsBodyAnchorX = achorPos.x * size.width;
	float physicsBodyAnchorY = -size.height + (achorPos.y * size.height);

	_physicsBody->setPositionOffset(Vector2(physicsBodyAnchorX, physicsBodyAnchorY));
}

void Entity::ResetMoveSpeed()
{
	SetCurrentMoveSpeed(m_baseMoveSpeed);
}

void Entity::PrepareForRespawn()
{
	if (m_animComponent != nullptr)
	{
		m_animComponent->PlayLoopingAnimation(GameConsts::ANIM_TYPE_IDLE);
	}

	ResetMoveSpeed();
	ResetStamina();
	ResetHealth();
	m_isTakingDamage = false;
	m_isDisappearing = false;
}

void Entity::ConsumeStamina(float amount)
{
	if (HasEnoughtStamina(amount))
	{
		m_stamina -= amount;
		StartStaminaRegenerateDelayTimer();
	}
	else
	{
		m_stamina = 0;
	}
	
	DispatchOnStaminaChangedEvent();
}

void Entity::TakeDamage(const Entity& attackingEntity)
{	
	if (m_health > 0)
	{		
		TakeDamage(attackingEntity.GetDamage());
		attackingEntity.DispatchOnGiveDamageEvent();		
		ApplyKnockbackEffect(attackingEntity);
	}
}

void Entity::TakeDamage(float damage)
{
	m_health -= damage;
	if (m_health < 0)
	{
		m_health = 0;
	}

	DispatchOnHealthReduceEvent();
	if (m_health > 0)
	{
		PlayHurtAnim();
	}
	else
	{
		PlayDissapearAnim();
	}
}

void Entity::StartAttacking()
{
	SetCurrentMoveSpeed(0.0f);
	m_isAttacking = true;
}

void Entity::StopAttacking()
{
	ResetMoveSpeed();
	m_isAttacking = false;
}

void Entity::ApplyKnockbackEffect(const Entity& attackingEntity)
{	
	const Vector2 awayFromAttacker = getPosition() - attackingEntity.getPosition();
	const float speed = attackingEntity.m_knockBackStrenght;
	ApplyInstantSpeedInDirection(speed, awayFromAttacker.getNormalized());
}

void Entity::ApplyInstantSpeed(float speed)
{
	ApplyInstantSpeedInDirection(speed, GetHeading());
}

void Entity::ApplyInstantSpeedInDirection(float speed, const Vector2& direction)
{
	_physicsBody->setVelocityLimit(speed);
	_physicsBody->setVelocity(direction * speed);
}

void Entity::Update(float deltaTime)
{
	Sprite::update(deltaTime);
	m_isRuning = m_moveDirection.lengthSquared() > 0;
	if (!m_isTakingDamage && !m_isDisappearing)
	{
		Move();
	}

	RegenerateStamina(m_staminaRegenerateSpeed * deltaTime);
	UpdateSortingLayer();
}

void Entity::StartStaminaRegenerateDelayTimer()
{
	m_isStaminaRegenerateDelayExpired = false;

	// Make sure any previous stamina regenerate actions are stopped
	stopActionByTag(GameConsts::ACTION_STAMINA_DELAY);

	// Start a new timer action
	Utils::StartTimerWithCallback(this,
		CC_CALLBACK_0(Entity::OnStaminaRegenerateDelayExpired, this),
			m_staminaRegenerateDelay, GameConsts::ACTION_STAMINA_DELAY);
}

void Entity::OnStaminaRegenerateDelayExpired()
{	
	m_isStaminaRegenerateDelayExpired = true;	
}

void Entity::OnDisappeared()
{
	DispatchOnDisappeared();
}

void Entity::PlayHurtAnim()
{
	if (m_animComponent != nullptr)
	{
		bool isCurrentlyPlayingHurtAnim = m_animComponent->IsCurrrentlyPlayingAnim(GameConsts::ANIM_TYPE_HURT) ||
			m_animComponent->IsCurrentlyPlayingDirAnim(GameConsts::ANIM_TYPE_HURT_DIR);

		if (!m_isAttacking && !isCurrentlyPlayingHurtAnim)
		{
			m_isTakingDamage = true;
			if (m_animComponent->HasAnim(GameConsts::ANIM_TYPE_HURT))
			{
				m_animComponent->PlayOneShotAnimation(GameConsts::ANIM_TYPE_HURT,
					CC_CALLBACK_0(Entity::OnDamageTaken, this));
			}
			else if (m_animComponent->HasAnim(GameConsts::ANIM_TYPE_HURT_DIR))
			{
				m_animComponent->PlayOneShotDirectionalAnim(GameConsts::ANIM_TYPE_HURT_DIR,
					CC_CALLBACK_0(Entity::OnDamageTaken, this));
			}
			else
			{
				// If no animation then fall-back to one second pause
				Utils::StartTimerWithCallback(this,
					CC_CALLBACK_0(Entity::OnDamageTaken, this),
					1.0f, GameConsts::ACTION_TAKE_DAMAGE);
			}
		}
	}
}

void Entity::PlayDissapearAnim()
{
	m_isDisappearing = true;
	if (m_animComponent != nullptr)
	{
		int animId = AnimationUtils::GetAnimId(GameConsts::ANIM_TYPE_DISAPPEAR);
		bool isAlreadyPlaying = m_animComponent->IsCurrrentlyPlayingAnim(animId);

		if (!isAlreadyPlaying)
		{
			if (m_animComponent->HasAnim(animId))
			{
				m_animComponent->PlayOneShotAnimation(animId, CC_CALLBACK_0(Entity::OnDisappeared, this));				
			}
			else
			{				
				OnDisappeared();
			}
		}		
	}
}

void Entity::Move()
{
	if (abs(m_moveDirection.x) > 0 || abs(m_moveDirection.y) > 0)
	{
		// Move entity by applying force
		_physicsBody->applyImpulse(m_moveDirection * m_moveSpeed * m_physicsBodyForceScale);		
	}	
	else if(!m_isAttacking)
	{
		// Instantly stop moving
		_physicsBody->setVelocity(Vector2::ZERO);
	}
}

void Entity::RegenerateStamina(float regenerateSpeedASecond)
{
	if (m_stamina < m_baseStamina && m_isStaminaRegenerateDelayExpired)
	{
		m_stamina += regenerateSpeedASecond;
		Utils::ClampValue(m_stamina, 0.0f, m_baseStamina);
		DispatchOnStaminaChangedEvent();
	}
}

void Entity::OnDamageTaken()
{
	m_isTakingDamage = false;
	m_animComponent->PlayLoopingAnimation(GameConsts::ANIM_TYPE_IDLE);
	ResetMoveSpeed();
}

void Entity::DispatchEvent(const String& eventType) const
{
	DispatchEvent(eventType, nullptr);
}

void Entity::DispatchEvent(const String& eventType, BaseEventData* eventData) const
{
	cc::EventDispatcher* eventDispatcher = getEventDispatcher();
	if (eventDispatcher != nullptr)
	{
		eventDispatcher->dispatchCustomEvent(eventType, eventData);
	}
}

void Entity::OnEntityInitialized()
{
	// Get animation component to trigger animations when that is necessary
	m_animComponent = static_cast<AnimComponent*>(getComponent(GameConsts::ANIM_COMPONENT));
	if (m_animComponent != nullptr)
	{
		m_animComponent->PlayLoopingAnimation(GameConsts::ANIM_TYPE_IDLE);
	}
	else
	{
		CCAssert(false, "Error: Did not find entity animation component !");
	}

	_physicsBody->setVelocityLimit(m_moveSpeed);
}

float Entity::GetCurrentMoveSpeed() const
{	
	return m_moveSpeed;
}

const Vector2& Entity::GetHeading() const
{	
	return m_moveDirection;
}

const cocos2d::Size& Entity::GetPhysicsBodySizeScaled() const
{
	return m_physicsBodyScaledSize;
}

float Entity::GetHealth() const
{
	return m_health;
}

float Entity::GetDamage() const
{
	return m_damage;
}

float Entity::GetCurrentHealth() const
{
	return m_health;
}

float Entity::GetCurrentStamina() const
{
	return m_stamina;
}

float Entity::GetMaxHealth() const
{
	return m_baseHealth;
}

float Entity::GetMaxStamina() const
{
	return m_baseStamina;
}

float Entity::GetPhysicsBodyForceScale() const
{
	return m_physicsBodyForceScale;
}

AnimComponent* Entity::GetAnimComponent() const
{
	return m_animComponent;
}

bool Entity::IsProcessing() const
{
	return m_isTakingDamage || m_isDisappearing;
}

unsigned Entity::GetId() const
{
	return m_id;
}

bool Entity::IsRunning() const
{
	return m_isRuning;
}

bool Entity::IsReadyToAttack() const
{
	return !m_isAttacking && !m_isTakingDamage;
}

bool Entity::HasEnoughtStamina(float amount) const
{
	return amount < m_stamina;
}

float Entity::GetKnockBackStrenght() const
{
	return m_knockBackStrenght;
}

float Entity::GetTimeModifier() const
{
	return getTimeModifier();
}

void Entity::SetPhysicsBodyForceScale(float scale)
{
	m_physicsBodyForceScale = scale;
}