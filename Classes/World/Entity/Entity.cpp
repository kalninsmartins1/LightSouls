#include "Entity.h"

NS_LIGHTSOULS_BEGIN

unsigned int Entity::s_uniqueId = 0;

Entity::Entity()	
	: m_id(s_uniqueId++)
	, m_moveDirection(Vector2::ZERO)
	, m_physicsBodyScaledSize(cocos2d::Size::ZERO)	
	, m_isRuning(false)
	, m_isAttacking(false)
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
{
}

Entity::~Entity()
{

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
	m_physicsBodyScaledSize.width = size.width * abs(getScaleX());
	m_physicsBodyScaledSize.height = size.height * abs(getScaleY());
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

void Entity::ConsumeStamina(float amount)
{
	if (HasEnoughtStamina(amount))
	{
		m_stamina -= amount;
	}
	else
	{
		m_stamina = 0;
	}
	
	DispatchOnStaminaChangedEvent();
}

void Entity::TakeDamage(float damage)
{
	if(m_health > 0)
	{
		m_health -= damage;
		if(m_health < 0)
		{
			m_health = 0;
		}
		
		DispatchOnHealthChangedEvent();
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

void Entity::update(float deltaTime)
{
	Sprite::update(deltaTime);
	m_isRuning = m_moveDirection.lengthSquared() > 0;
	Move();

	RegenerateStamina(m_staminaRegenerateSpeed * deltaTime);
}

void Entity::DispatchOnHealthChangedEvent()
{
	// Does nothing by default
}

void Entity::DispatchOnStaminaChangedEvent()
{
	// Does nothing by default
}

void Entity::Move()
{
	if (abs(m_moveDirection.x) > 0 || abs(m_moveDirection.y) > 0)
	{
		// Move entity by applying force
		_physicsBody->applyImpulse(m_moveDirection * m_moveSpeed * m_physicsBodyForceScale);		
	}	
	else
	{
		// Instantly stop moving
		_physicsBody->setVelocity(Vector2::ZERO);
	}
}

void Entity::RegenerateStamina(float regenerateSpeedASecond)
{
	if (m_stamina < m_baseStamina)
	{
		m_stamina += regenerateSpeedASecond;
		DispatchOnStaminaChangedEvent();
	}
}

void Entity::OnEntityInitialized()
{
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

unsigned Entity::GetId() const
{
	return m_id;
}

bool Entity::IsRunning() const
{
	return m_isRuning;
}

bool Entity::IsAttacking() const
{
	return m_isAttacking;
}

bool Entity::HasEnoughtStamina(float amount)
{
	return amount < m_stamina;
}

void Entity::SetPhysicsBodyForceScale(float scale)
{
	m_physicsBodyForceScale = scale;
}

NS_LIGHTSOULS_END